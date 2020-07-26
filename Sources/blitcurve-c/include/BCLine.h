// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#include "BCTypes.h"
#include "BCMacros.h"
#include "BCMetalC.h"

#ifndef __METAL_VERSION__
#include <math.h>
#include <assert.h>
#include <simd/simd.h>
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
    __BC_ASSERT(fabsf(diff.x) > 0);
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
    __BC_ASSERT(BCLineLength(l) > 0);
        bc_float2_t diff = l.b - l.a;
    return atan2(diff.y, diff.x);
}


///\abstract 'Evaluates' the line with the 'bezier parameter' [0..1]'
///\returns \c a for \c 0, \c b for \c 1, and otherwise a linear interpolation along the line
__attribute__((const))
__attribute__((swift_name("Line.evaluate(self:t:)")))
static inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t) {
    __BC_ASSERT(t >= 0.0 && t <= 1.0);
    return simd_mix(l.a,l.b,simd_make_float2(t,t));
}

///Performs an arclength parameterization.  This finds a bezier parameter \c t (in range 0,1) that is a length specified from \c line.a.
///\performance This is O(1).
__attribute__((const))
__attribute__((swift_name("Line.parameterization(self:arclength:)")))
static inline bc_float_t BCLineArclengthParameterization(BCLine c, bc_float_t distance) {
    __BC_ASSERT(BCLineLength(c) >= distance);
    return distance / BCLineLength(c);
}


#endif /* Line_h */
