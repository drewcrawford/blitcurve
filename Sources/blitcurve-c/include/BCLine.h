// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#import <simd/simd.h>
#import "BCTypes.h"
#import <math.h>
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
///- note: Vertical lines may be a large value or NaN
__attribute__((const))
__attribute__((swift_name("getter:Line.slope(self:)")))
inline bc_float_t BCLineSlope(BCLine l) {
    return (l.a.y - l.b.y) / (l.a.x - l.b.x);
}


#endif /* Line_h */
