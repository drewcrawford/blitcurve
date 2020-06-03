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
    l.b.x = distance * cosf(angle) + a.x;
    l.b.y = distance * sinf(angle) + a.y;
    return l;
}

///Gets the slope of the line.
///- note: Vertical lines are undefined
__attribute__((const))
__attribute__((swift_name("getter:Line.slope(self:)")))
inline bc_float_t BCLineSlope(BCLine l) {
    ASSERT_UB(fabsf(l.a.x - l.b.x) > 0);
    return (l.a.y - l.b.y) / (l.a.x - l.b.x);
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

__swift_unavailable("Not available")
inline bc_float_t _BCLineTangentUnchecked(BCLine l) {
    return atan2f(l.b.y - l.a.y, l.b.x - l.a.x);
}

///Tangent along the line.
///- warning: In the case the line has 0 length, this is UB
__attribute__((const))
__attribute__((swift_name("getter:Line.tangent(self:)")))
inline bc_float_t BCLineTangent(BCLine l) {
    ASSERT_UB(BCLineDistance(l) > 0);
    return _BCLineTangentUnchecked(l);
}


///'Evaluates' the line with the 'bezier parameter' [0..1]'
///This returns a for 0, b for 1, and otherwise a linear interpolation along the line
//__attribute__((const))
__attribute__((swift_name("Line.evaluate(self:t:)")))
inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t) {
    //this is OK because atanf does return a non-nan value for 0 length
    //and we multiply it by 0
    bc_float_t tangent = _BCLineTangentUnchecked(l);
    bc_float2_t term_1 = simd_make_float2(cosf(tangent),sinf(tangent));
    bc_float2_t term_2 = term_1 * t * BCLineDistance(l);
    return l.a.x + term_2;
}


#endif /* Line_h */
