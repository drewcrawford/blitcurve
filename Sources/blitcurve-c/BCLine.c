// BCLine.c: Line type
// ©2020 DrewCrawfordApps LLC
#import "BCLine.h"

extern inline BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance);
extern inline bc_float_t BCLineSlope(BCLine l);
