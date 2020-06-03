// BCLine.c: Line type
// ©2020 DrewCrawfordApps LLC
#import "BCLine.h"

extern inline BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance);
extern inline bc_float_t BCLineSlope(BCLine l);
extern inline bc_float_t BCLineYIntercept(BCLine l);
extern inline bc_float_t BCLineDistance(BCLine l);
extern inline bc_float_t BCLineTangent(BCLine l);
extern inline bc_float2_t BCLineEvaluate(BCLine l,bc_float_t t);
