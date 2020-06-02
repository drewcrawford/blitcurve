// BCLine.h: Line type
// ©2020 DrewCrawfordApps LLC

#ifndef Line_h
#define Line_h
#import <simd/simd.h>
#import "BCTypes.h"
__attribute__((swift_name("Line")))
typedef struct {
    bc_float2_t a;
    bc_float2_t b;
} BCLine;

__attribute__((const))
__attribute__((swift_name("Line.init(point:angle:distance:)")))
BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance);
#endif /* Line_h */
