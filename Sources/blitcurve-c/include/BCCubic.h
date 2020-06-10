// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC

#ifndef Cubic_h
#define Cubic_h
#import "BCMetalC.h"

#import "BCTypes.h"
#import "BCLine.h"
#import "BCMath.h"
#import "BCLine2.h"

#ifndef __METAL_VERSION__
#include <stdbool.h>
#endif

#include <simd/simd.h>
///BCCubic is a cubic bezier curve defined on 4 points.
///Unlike some other notation, we use `a` and `b` consistently for start and end points, reserving other values for control points.
__attribute__((swift_name("Cubic")))
typedef struct {
    ///start of curve
    bc_float2_t a;
    ///end of curve
    bc_float2_t b;
    ///control point 1
    bc_float2_t c;
    ///control point 2
    bc_float2_t d;
} BCCubic;

///Returns the line from a to c.
///- warning: In the case that a =~= c, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentLine(self:)")))
static inline BCLine BCCubicInitialTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.c;
    return l;
}

///Returns the line from d to b.
///- warning: In the case that d=~= b, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentLine(self:)")))
static inline BCLine BCCubicFinalTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.d;
    l.b = c.b;
    return l;
}

///Returns the angle of the intitalTangent.
///- warning: For non-normalized cubic, this may be UB
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangent(self:)")))
static inline bc_float_t BCCubicInitialTangent(BCCubic c) {
    return BCLineTangent(BCCubicInitialTangentLine(c));
}

///Returns the angle of the finalTangent.
///- warning: For non-normalized cubic, this may be UB
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
static inline bc_float_t BCCubicFinalTangent(BCCubic c) {
    return BCLineTangent(BCCubicFinalTangentLine(c));
}

///Evaluate the cubic for the given bezier parameter
///- returns: A point on the cubic at the bezier equation for t
__attribute__((const))
__attribute__((swift_name("Cubic.evaluate(self:t:)")))
static inline bc_float2_t evaluate(BCCubic c,bc_float_t t) {
    return c.a * pow(1-t,3) + c.c * 3 * pow(1-t, 2) * t + c.d * 3 * (1 - t) * pow(t, 2) + c.b * pow(t, 3);
}

__attribute__((const))
__attribute__((swift_name("getter:Cubic.asLine(self:)")))
static inline BCLine BCCubicAsLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.b;
    return l;
}

///For various reaons, a cubic that has its extremity points close to its control points are problematic.
///For example, the initial/final tangents of such a cubic are undefined.
///This function will normalize the cubic by adjusting its control points if the cubic is badly-behaved.
__attribute__((swift_name("Cubic.normalize(self:)")))
static inline void BCCubicNormalize(BCCubic __BC_DEVICE *c) {
    if (BCIsNearlyEqual2(c->a,c->c)) {
        BCLine asLine = BCCubicAsLine(*c);
        c->c = BCLineEvaluate(asLine,0.001);
    }
    if (BCIsNearlyEqual2(c->d,c->b)) {
        BCLine asLine = BCCubicAsLine(*c);
        c->d = BCLineEvaluate(asLine,0.999);
    }
}

///Create a cubic that connects a line with initial and final tangents
///- warning: This may be UB ifconnecting line is 0-length
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:initialTangent:finalTangent:)")))
static inline BCCubic BCCubicMakeConnectingTangents(BCLine connecting, bc_float_t initialTangent, bc_float_t finalTangent) {
    BCCubic c;
    c.a = connecting.a;
    c.b = connecting.b;
    
    /*
                
    pick c/d such that it's collinear to the tangent
    keep in mind the following 5 cases
      ▲               ▲
       ╲               ╲
       ft    .          ft   .
         ╲  ( )          ╲  ( )
          ╲  '            ╲  '
          ▲               ▲
          │               │
        line             line
          │               │
          │   .       .   │
          ▲  ( )     ( )  ▲
         ╱    '       '    ╲
       it               it  ╲
       ╱                     ╲
      ╱                       ╲
                               ╲
                                                    
                                              ▲
      .     ▲               ▲                 │
     ( )   ╱               ╱                  │
      '   ╱         . ft  ╱                 ft│
     ft  ╱         ( )   ╱                    │
        ╱           '   ╱                     │
        ▲               ▲                     ▲.
        │               │                     ( )
        │              line                  l.'e
      line              │                    ( )
        │   .       .   │                     '
        ▲  ( )     ( )  ▲                     ▲
       ╱    '       '    ╲                    │
      ╱               it  ╲                 it│
     it                    ╲                  │
    ╱                       ╲                 │
                             ╲                │
    
    The core idea here is we try to figure out the IT/FT are to the right or left of the line
    and place the control point on the opposite side.
     Note that to get ft like it is in this diagram, we need to reverse the arg
     
        */
    bc_float_t lineTangent = BCLineTangent(connecting);
    //find the difference between angles in (i,f) format
    //we subtract by M_PI here to reverse the finalTangent we were given
    bc_float2_t diff = simd_make_float2(initialTangent, finalTangent - M_PI);
    BCLine2 i_f = BCLine2MakeWithPointAndAngle(simd_make_float4(connecting.a,connecting.b),diff,simd_make_float2(1,1));
    c.c = i_f.b.xy;
    c.d = i_f.b.zw;
    return c;
}

///Creates a cubic connecting two cubics, with an initialTangent [finalTangent of the a] and finalTangent [reversed initialTangent of B]
///- warning: UB if the cubics are not normalized
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:)")))
static inline BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b.a;
    return BCCubicMakeConnectingTangents(connecting, BCCubicFinalTangent(a), BCCubicInitialTangent(b) + M_PI_2);
}

///Calculates the arclength.
///- note: This is a fast numerical and vectorized approximation for the arc length.
///- complexity: O(1)
__attribute__((const))
__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic c);
/*

__attribute__((swift_name("Cubic.init(connecting:to:)")))
BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b);

__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic a);

__attribute__((swift_name("Cubic.hasIntersection(self:with:)")))
bool BCCubicHasIntersection(BCCubic a, BCCubic b);
 */

#endif

