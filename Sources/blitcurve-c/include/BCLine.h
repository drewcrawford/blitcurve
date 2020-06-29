// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#import "BCTypes.h"
#import "BCMacros.h"
#import "BCMetalC.h"

#ifndef __METAL_VERSION__
#import <math.h>
#import <assert.h>
#import <simd/simd.h>
#endif
///\abstract BCLine is a linesegment defined on 2 points
///\discussion Unlike some other notation, we use \c a and  \c b consistently for start and end points, reserving other values for control points.
__attribute__((swift_name("Line")))
typedef struct {
    ///start point
    bc_float2_t a;
    ///end point
    bc_float2_t b;
} BCLine;


__attribute__((const))
__attribute__((swift_name("Line.init(point:angle:distance:)")))
///\abstract Create a line with the start point and angle
///\param a start point of the line
///\param angle angle from the start point
///\param distance distance from the start point
static inline BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance) {
    BCLine l;
    l.a = a;
    l.b = simd_make_float2(cos(angle), sin(angle));
    l.b *= distance;
    l.b += a;
    return l;
}


__attribute__((const))
__attribute__((swift_name("getter:Line.slope(self:)")))
///\abstract Gets the slope of the line.
///\warning Vertical lines are undefined
static inline bc_float_t BCLineSlope(BCLine l) {
    bc_float2_t diff = l.a - l.b;
    BC_ASSERT(fabsf(diff.x) > 0);
    return diff.y / diff.x;
}

///Y-intercept of the line
///\warning Vertical lines are undefined
__attribute__((const))
__attribute__((swift_name("getter:Line.yIntercept(self:)")))
static inline bc_float_t BCLineYIntercept(BCLine l) {
    return l.a.y - BCLineSlope(l) * l.a.x;
}

///Distance of the line
__attribute__((const))
__attribute__((swift_name("getter:Line.length(self:)")))
static inline bc_float_t BCLineLength(BCLine l) {
    return simd_fast_length(l.a - l.b);
}

///Reverse start and end point
__attribute__((const))
__attribute__((swift_name("getter:Line.reversed(self:)")))
static inline BCLine BCLineReversed(BCLine l) {
    BCLine r;
    r.a = l.b;
    r.b = l.a;
    return r;
}


///Tangent along the line.
///\warning In the case the line has 0 length, this is UB
__attribute__((const))
__attribute__((swift_name("getter:Line.tangent(self:)")))
static inline bc_float_t BCLineTangent(BCLine l) {
    BC_ASSERT(BCLineLength(l) > 0);
        bc_float2_t diff = l.b - l.a;
    return atan2(diff.y, diff.x);
}


///\abstract 'Evaluates' the line with the 'bezier parameter' [0..1]'
///\returns \c a for \c 0, \c b for \c 1, and otherwise a linear interpolation along the line
//__attribute__((const))
__attribute__((swift_name("Line.evaluate(self:t:)")))
static inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t) {
    BC_ASSERT(t >= 0.0 && t <= 1.0);
    return simd_mix(l.a,l.b,simd_make_float2(t,t));
}


#endif /* Line_h */
