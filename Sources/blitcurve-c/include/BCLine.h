// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#import <simd/simd.h>
#import "BCTypes.h"
#import "BCMacros.h"
#import <math.h>
#import <assert.h>
///BCLine is a linesegment defined on 2 points
///Unlike some other notation, we use `a` and `b` consistently for start and end points, reserving other values for control points.
__attribute__((swift_name("Line")))
typedef struct {
    bc_float2_t a;
    bc_float2_t b;
} BCLine;

__attribute__((const))
__attribute__((swift_name("Line.init(point:angle:distance:)")))
inline BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance) {
    BCLine l;
    l.a = a;
    l.b = simd_make_float2(cosf(angle), sinf(angle));
    l.b *= distance;
    l.b += a;
    return l;
}

///Gets the slope of the line.
///- note: Vertical lines are undefined
__attribute__((const))
__attribute__((swift_name("getter:Line.slope(self:)")))
inline bc_float_t BCLineSlope(BCLine l) {
    bc_float2_t diff = l.a - l.b;
    ASSERT_UB(fabsf(diff.x) > 0);
    return diff.y / diff.x;
}

///Y-intercept of the line
///- note: Vertical lines are undefined
__attribute__((const))
__attribute__((swift_name("getter:Line.yIntercept(self:)")))
inline bc_float_t BCLineYIntercept(BCLine l) {
    return l.a.y - BCLineSlope(l) * l.a.x;
}

///Distance of the line
__attribute__((const))
__attribute__((swift_name("getter:Line.distance(self:)")))
inline bc_float_t BCLineDistance(BCLine l) {
    return simd_fast_length(l.a - l.b);
}

///Reverse start and end point
__attribute__((const))
__attribute__((swift_name("getter:Line.reversed(self:)")))
inline bc_float_t BCLineReversed(BCLine l) {
    return simd_fast_length(l.a - l.b);
}


///Tangent along the line.
///- warning: In the case the line has 0 length, this is UB
__attribute__((const))
__attribute__((swift_name("getter:Line.tangent(self:)")))
inline bc_float_t BCLineTangent(BCLine l) {
    ASSERT_UB(BCLineDistance(l) > 0);
        bc_float2_t diff = l.b - l.a;
    return atan2f(diff.y, diff.x);
}


///'Evaluates' the line with the 'bezier parameter' [0..1]'
///This returns a for 0, b for 1, and otherwise a linear interpolation along the line
//__attribute__((const))
__attribute__((swift_name("Line.evaluate(self:t:)")))
inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t) {
    ASSERT_UB(t >= 0.0 && t <= 1.0);
    return simd_mix(l.a,l.b,simd_make_float2(t,t));
}


#endif /* Line_h */
