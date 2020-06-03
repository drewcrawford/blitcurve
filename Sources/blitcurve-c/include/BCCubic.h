// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC

#ifndef Cubic_h
#define Cubic_h
#import "BCTypes.h"
#import "BCLine.h"
#import "BCMath.h"
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
__attribute__((swift_name("getter:Cubic.initialTangent(self:)")))
inline BCLine BCCubicInitialTangent(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.c;
    return l;
}

///Returns the line from d to b.
///- warning: In the case that d ~= b, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
inline BCLine BCCubicFinalTangent(BCCubic c) {
    BCLine l;
    l.a = c.d;
    l.b = c.b;
    return l;
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

/*

__attribute__((swift_name("Cubic.init(connecting:to:)")))
BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b);

__attribute__((swift_name("Cubic.init(connecting:initialTangent:finalTangent:)")))
BCCubic BCCubicMakeConnectingTangents(BCCubic connecting, BCCubic initialTangent, BCCubic finalTangent);

__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic a);

__attribute__((swift_name("Cubic.hasIntersection(self:with:)")))
bool BCCubicHasIntersection(BCCubic a, BCCubic b);

__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
BCLine LCCubicFinalTangent(BCCubic a);
 */

#endif

