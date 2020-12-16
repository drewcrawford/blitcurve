// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#include "BCTypes.h"
#include "BCMacros.h"
#include "BCMetalC.h"
#include "BCTrap.h"

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
static inline BCLine BCLineMakeWithPointAndAngle(bc_float2_t a, float angle, float distance) {
    BCLine l;
    l.a = a;
    l.b = bc_make_float2(bc_cos(angle), bc_sin(angle));
    l.b *= distance;
    l.b += a;
    return l;
}


__attribute__((const))
__attribute__((swift_name("getter:Line.slope(self:)")))
/**\abstract Gets the slope of the line.
\throws Vertical lines trap with assert.  rvalue is \c BC_FLOAT_LARGE.
 */
inline bc_float_t BCLineSlope(BCLine l) {
    bc_float2_t diff = l.a - l.b;
    __BC_ASSERT2(bc_abs(diff.x) > 0, BC_FLOAT_LARGE);
    return diff.y / diff.x;
}

/**Y-intercept of the line
 \throws Vertical lines trap with assert.  rvalue is \c BC_FLOAT_LARGE.
 */
__attribute__((const))
__attribute__((swift_name("getter:Line.yIntercept(self:)")))
inline bc_float_t BCLineYIntercept(BCLine l) {
    __BC_ASSERT2(bc_abs(l.a.x-l.b.x) > 0, BC_FLOAT_LARGE);
    return l.a.y - BCLineSlope(l) * l.a.x;
}

///Distance of the line
__attribute__((const))
__attribute__((swift_name("getter:Line.length(self:)")))
static inline bc_float_t BCLineLength(BCLine l) {
    return bc_fast_length(l.a - l.b);
}

///Reverse start and end point
__attribute__((const))
__attribute__((swift_name("getter:Line.reversed(self:)")))
inline BCLine BCLineReversed(BCLine l) {
    BCLine r;
    r.a = l.b;
    r.b = l.a;
    return r;
}


/**Tangent along the line.
\warning throws if the line has \c 0 length.  rvalue is \c BC_FLOAT_LARGE.
 */
__attribute__((const))
__attribute__((swift_name("getter:Line.tangent(self:)")))
static inline bc_float_t BCLineTangent(BCLine l) {
    __BC_ASSERT2(BCLineLength(l) > 0,BC_FLOAT_LARGE);
    const bc_float2_t diff = l.b - l.a;
    return bc_atan2(diff.y, diff.x);
}


/**
 \abstract 'Evaluates' the line with the 'bezier parameter' [0..1]'
 \throws Asserts to check arguments, rvalue is \c {BC_FLOAT_LARGE,BC_FLOAT_LARGE}.
\returns \c a for \c 0, \c b for \c 1, and otherwise a linear interpolation along the line
 */
__attribute__((const))
__attribute__((swift_name("Line.evaluate(self:t:)")))
static inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t) {
    __BC_ASSERT2(t >= 0.0 && t <= 1.0, bc_make_float2(BC_FLOAT_LARGE,BC_FLOAT_LARGE));
    return bc_mix(l.a,l.b,bc_make_float2(t,t));
}

/**
 Performs an arclength parameterization.  This finds a bezier parameter \c t (in range 0,1) that is a length specified from \c line.a.
\performance This is O(1).
 \throws Asserts to check arguments, rvalue is \c (-1-BCError).
 */
__attribute__((const))
__attribute__((swift_name("Line.parameterization(self:arclength:)")))
static inline bc_float_t BCLineArclengthParameterization(BCLine c, bc_float_t distance) {
    __BC_ASSERT2(distance >= 0, (1-BCErrorArg1));
    const float lineLength = BCLineLength(c);
    __BC_ASSERT2(lineLength > 0,(1-BCErrorSizeIsZero));
    __BC_ASSERT2(lineLength >= distance,(1-BCErrorArgRelationship))
    return distance / lineLength;
}


#endif /* Line_h */
