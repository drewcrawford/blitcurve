// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC

#ifndef Cubic_h
#define Cubic_h
#import "BCTypes.h"
#import "BCLine.h"
#import "BCMath.h"
#import "BCLine2.h"
#include <stdbool.h>
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
///- warning: In the case that a ~= c, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentLine(self:)")))
inline BCLine BCCubicInitialTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.c;
    return l;
}

///Returns the line from d to b.
///- warning: In the case that d ~= b, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentLine(self:)")))
inline BCLine BCCubicFinalTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.d;
    l.b = c.b;
    return l;
}

///Returns the angle of the intitalTangent.
///- warning: For non-normalized cubic, this may be UB
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangent(self:)")))
inline bc_float_t BCCubicInitialTangent(BCCubic c) {
    return BCLineTangent(BCCubicInitialTangentLine(c));
}

///Returns the angle of the finalTangent.
///- warning: For non-normalized cubic, this may be UB
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
inline bc_float_t BCCubicFinalTangent(BCCubic c) {
    return BCLineTangent(BCCubicFinalTangentLine(c));
}

///Evaluate the cubic for the given bezier parameter
///- returns: A point on the cubic at the bezier equation for t
__attribute__((const))
__attribute__((swift_name("Cubic.evaluate(self:t:)")))
inline bc_float2_t evaluate(BCCubic c,bc_float_t t) {
    return c.a * powf(1-t,3) + c.c * 3 * powf(1-t, 2) * t + c.d * 3 * (1 - t) * powf(t, 2) + c.b * powf(t, 3);
}

__attribute__((const))
__attribute__((swift_name("getter:Cubic.asLine(self:)")))
inline BCLine BCCubicAsLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.b;
    return l;
}

///For various reaons, a cubic that has its extremity points close to its control points are problematic.
///For example, the initial/final tangents of such a cubic are undefined.
///This function will normalize the cubic by adjusting its control points if the cubic is badly-behaved.
__attribute__((swift_name("Cubic.normalize(self:)")))
inline void BCCubicNormalize(BCCubic *c) {
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
__attribute__((swift_name("Cubic.init(connecting:initialTangent:finalTangent:)")))
inline BCCubic BCCubicMakeConnectingTangents(BCLine connecting, bc_float_t initialTangent, bc_float_t finalTangent) {
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
__attribute__((swift_name("Cubic.init(connecting:to:)")))
inline BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b.a;
    return BCCubicMakeConnectingTangents(connecting, BCCubicFinalTangent(a), BCCubicInitialTangent(b) + M_PI_2);
}

///Calculates the arclength.
///- note: This is a fast numerical and vectorized approximation for the arc length.
///- complexity: O(1)
__attribute__((swift_name("getter:Cubic.length(self:)")))
inline bc_float_t BCCubicLength(BCCubic c) {
    /*as far as I'm aware, the implementation here has some unique properties I have not seen in other bezier-curve sources.
    the core idea here is a Lobatto quadrature for n=5, to integrate the length-of-derivative.  That constant-time solution is well-known in the literature, although not widely used from what I can see.
     https://mathworld.wolfram.com/LobattoQuadrature.html
     
     The innovation here is to reimagine the quadrature in a matrix notation, for which fast vectorized primitves are available.  The result is a vectorized implementation which is faster than the usual methods.
     
    
        for v1,v3 we want to compute some equation like
    v1.x = abs(-0.558983582205757*c0.x + 0.325650248872424*c1.x + 0.208983582205757*c2.x + 0.024349751127576*c3.x)
    v3.x abs(-0.024349751127576*c0.x - 0.208983582205757*c1.x - 0.325650248872424*c2.x + 0.558983582205757*c3.x)
     
     all where c0=a,c1=c,c2=d,c3=b
     let's call coefficiencts j*c0 + k*c1 + l*c2 + m * c3
    The part inside the abs can be represented as a matrix multiplication with
                                   ┌────────┐
           ┌─────────────────┐     │j1,j3   │      ┌────────┐
           │c0x,c1x,c2x,c3x  │     │k1,k3   │  =   │v1x,v3x │
           │c0y,c1y,c2y,c3y  │  x  │l1,l3   │      │v1y,v3y │
           └─────────────────┘     │ m1,m3  │      └────────┘
                                   │        │
                 A                 └────────┘
                                                    C
                                      B
     Note that simd uses a column-major oder

     */
    simd_float4 v1_coef = simd_make_float4(-0.558983582205757, 0.325650248872424, 0.208983582205757,0.024349751127576);
    simd_float4 v3_coef = simd_make_float4(-0.024349751127576, -0.208983582205757, -0.325650248872424, 0.558983582205757);
    
    simd_float4x2 mtxA = simd_matrix(c.a, c.c,c.d,c.b);
    simd_float2x4 mtxB = simd_matrix(v1_coef, v3_coef);

    simd_float2x2 mtxC = simd_mul(mtxA, mtxB);
    
    
    /*we could potentially use a similar trick to calculate the simpler expressions
     v2_x = abs(c3.x-c0.x+c2.x-c1.x)*0.26666666666666666
    v0_x = abs(c1.x-c0.x)*0.15
    and
    v4_x = abs(c3.x-c2.x) * 0.15
     
     However v0 and v4 only refer to 2 coefficients each, so we are wasting a bit.  Also we can implement this without any multiplication via this method
      0    1   2   3       0    1   2  3           0   1    2    3
     ┌────────────────┐   ┌───────────────┐      ┌───────────────────┐
     │c1x c3x c3x c2x │   │c0x c2x c0x c1x│      | v0x v4x v21x v22x │
     │c1y c3y c3y c2y │ - │c0y c1y c0x c1y│ =    │ v0y v4y v22y v22y │
     └────────────────┘   └───────────────┘      │                   │
                                                 └───────────────────┘
     D                      E                     F
    */
    
    simd_float4x2 mtxD = simd_matrix(c.c, c.b, c.b, c.d);
    simd_float4x2 mtxE = simd_matrix(c.a, c.d, c.a, c.c);
    simd_float4x2 mtxF = simd_sub(mtxD, mtxE);
    
    //to complete v2, we need to get v21 + v22
    simd_float2 v2 = mtxF.columns[2] + mtxF.columns[3];
    
    /*at this point, we need to calculate our absolute values
    across v0,v1,v2,v3,v4 x 2 dimensions (10 in total)
     
     
     We assemble these in a strange order.  v0,v2,v4 require some additional multiplication.
     v0,v4 will be 0.15
     v2 will be 0.26...
     
     while v1,v3 will be complete after abs.
    */
    simd_float4 a_v0_v4 = simd_make_float4(mtxF.columns[0],mtxF.columns[1]);
    simd_float4 a_v2_v1 = simd_make_float4(v2,mtxC.columns[0]);
    simd_float2 a_v3 = mtxC.columns[1];
    
    //note: simd_float8/16 is not available in Metal
    //need a 4-wide abs solution for that case, we're just assembling and taking the abs of these 10 values
    simd_float8 a8_v0_v4_v2_v1 = simd_make_float8(a_v0_v4,a_v2_v1);
    simd_float8 a8_v3 = simd_make_float8_undef(a_v3);
    simd_float16 a16_v0_v4_v2_v1_v3 = simd_make_float16(a8_v0_v4_v2_v1,a8_v3);
    simd_float16 takenAbs_v0_v4_v2_v1_v3 = simd_abs(a16_v0_v4_v2_v1_v3);
    
    /*Now wthe additional multiplication
     
     Note that we're now in vector notation
     
     we want to take
     v0x,v0y,v2x,v2y,v4x,v4y
     
     x
     
     0.15,0.15,0.15,0.15,0.26,0.26
     
                                                      
     
     */
    simd_float8 low8 = takenAbs_v0_v4_v2_v1_v3.lo;
    simd_float4 v0_4_coef = simd_make_float4(0.15,0.15,0.15,0.15);
    //we are only multiplying by 6 terms so we don't care about the last 2
    simd_float4 v2_dc_coef = simd_make_float4_undef(simd_make_float2(0.26666666666666666,0.26666666666666666));
    
    simd_float8 final_coef = simd_make_float8(v0_4_coef, v2_dc_coef);
    simd_float8 v0_v4_v2_dc = low8 * final_coef;
    
    //get v1 and v3
    simd_float2 v1 = takenAbs_v0_v4_v2_v1_v3.lo.hi.hi;
    simd_float2 v3 = takenAbs_v0_v4_v2_v1_v3.hi.xy;
    
    /*To do our final reduction, we need to add v0,v1,v2,v3,v4 (5 terms) in 2 sets (seprately for x,y)
     
     we could implement this like
     
     simd8 x = v0,v1,v2,v3,v4,0,0,0
     simd8 y = v0,v1,v2,v3,v4,0,0,0
     
     reduce(x)
     reduce(y)
     
     which has 3 wasted additions.  Or we could implement
     
     simd4 x_0 = v0,v1,v2,v3
     simd4 y_0 = v0,v1,v2,v3
     reduce(x_0)
     reduce(y_0)
     
     x_0,y_0 + (v4)
     
     Trying the first I guess, it's illegal in metal but we could potentially compare with #2
     
     */
    
    //split x and y
    simd_float4 x_v0_v4_v2_v1 = v0_v4_v2_dc.even;
    simd_float4 y_v0_v4_v2_v1 = v0_v4_v2_dc.odd;
    
    //put v1 into 1
    x_v0_v4_v2_v1.w = v1.x;
    y_v0_v4_v2_v1.w = v1.y;
    
    simd_float4 x_v3_0 = simd_make_float4(v3.x,0,0,0);
    simd_float4 y_v3_0 = simd_make_float4(v3.y,0,0,0);
    simd_float8 vx = simd_make_float8(x_v0_v4_v2_v1, x_v3_0);
    simd_float8 vy = simd_make_float8(y_v0_v4_v2_v1, y_v3_0);
    
    bc_float_t reduceX = simd_reduce_add(vx);
    bc_float_t reduceY = simd_reduce_add(vy);
    bc_float2_t reduce2D = simd_make_float2(reduceX,reduceY);
    return simd_length(reduce2D);
}

/*

__attribute__((swift_name("Cubic.init(connecting:to:)")))
BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b);

__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic a);

__attribute__((swift_name("Cubic.hasIntersection(self:with:)")))
bool BCCubicHasIntersection(BCCubic a, BCCubic b);
 */

#endif

