// BCLine2.h: Double line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line2_h
#define Line2_h
#import "BCTypes.h"
///BCLine is logically 2 BCLine.  In certain cases we can perform operations faster via vectorization.
///Unlike some other notation, we use `a` and `b` consistently for start and end points, reserving other values for control points.
__attribute__((swift_name("Line2")))
typedef struct {
    ///First point of both lines.  {x,w} are x and {y,z} are y.  Alternatively, "high half" is one point and "low half" is the other.
    bc_float4_t a;
    ///Second point of both lines. {x,w} are x and {y,z} are y.  Alternatively, "high half" is one point and "low half" is the other.
    bc_float4_t b;
} BCLine2;

///Create Line2 from start-points and angles.
__attribute__((const))
__attribute__((swift_name("Line2.init(point:angle:distance:)")))
static inline BCLine2 BCLine2MakeWithPointAndAngle(bc_float4_t a, bc_float2_t angle, bc_float2_t distance) {
    BCLine2 l;
    l.a = a;
    bc_float2_t cos = cos(angle);
    bc_float2_t sin = sin(angle);
    l.b = simd_make_float4(cos.x,sin.x,cos.y,sin.y);
    l.b *= simd_make_float4(distance.x,distance.x,distance.y,distance.y);
    l.b += a;
    return l;
}
#endif
