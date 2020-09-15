// BCCubic.c: Cubic implementation
// ©2020 DrewCrawfordApps LLC

#include "BCCubic.h"
#include "BCMetalC.h"
extern inline BCLine BCCubicInitialTangentLine(BCCubic c);
extern inline BCLine BCCubicFinalTangentLine(BCCubic c);
extern inline bc_float2_t BCCubicEvaluate(BCCubic c,bc_float_t t);
extern inline BCLine BCCubicAsLine(BCCubic c) ;

bc_float_t BCCubicLength(BCCubic c) {
    const simd_float2 v0 = simd_abs(c.c-c.a);
    const simd_float2 v1 = simd_abs(-0.558983582205757*c.a + 0.325650248872424*c.c + 0.208983582205757*c.d + 0.024349751127576*c.b);
    const simd_float2 v2 = simd_abs(c.b-c.a+c.d-c.c)*0.26666666666666666;
    const simd_float2 v3 = simd_abs(-0.024349751127576*c.a - 0.208983582205757*c.c - 0.325650248872424*c.d + 0.558983582205757*c.b);
    const simd_float2 v4 = simd_abs(c.b-c.d);

    const simd_float2 result = 0.15*(v0+v4) + v1 + v2 + v3;
    return simd_length(result);
}

BCCubic BCCubicLeftSplit(BCCubic c, bc_float_t t) {
    BCCubic out;
    const bc_float_t t_minus_1 = t - 1;
    const bc_float_t t_minus_1_squared = powf(t_minus_1,2);
    const bc_float2_t t_squared_d = powf(t,2) * c.d;
    const bc_float2_t t_c = t * c.c;
    out.a = c.a;
    out.b = powf(t,3) * c.b - 3 * t_squared_d * t_minus_1 + 3 * t_minus_1_squared * t_c - powf(t_minus_1,3) * c.a;
    out.c = t_c - t_minus_1 * c.a;
    out.d = t_squared_d - 2 * t_minus_1 * t_c + t_minus_1_squared * c.a;
    return out;
}

BCCubic BCCubicRightSplit(BCCubic c, bc_float_t t) {
    BCCubic out;
    const bc_float_t t_squared = powf(t,2);
    const bc_float2_t t_minus_1_d = (t-1) * c.d;
    const bc_float2_t t_minus_1_squared_c = powf(t - 1,2) * c.c;
    out.a = powf(t,3) * c.b - 3 * t_squared * t_minus_1_d  + 3 * t * t_minus_1_squared_c - powf(t - 1,3) * c.a;
    out.b = c.b;
    out.c = t_squared * c.b - 2 * t * t_minus_1_d + t_minus_1_squared_c;
    out.d = t * c.b - t_minus_1_d;
    return out;
}

BCCubic2 BCCubicSplit(BCCubic c, bc_float_t t) {
    const bc_float_t t_minus_1 = t - 1;
    const bc_float2_t t_minus_1_d = t_minus_1 * c.d;
    const bc_float2_t t_c = t * c.c;
    const bc_float_t t_minus_1_squared = powf(t_minus_1, 2);
    const bc_float2_t t_minus_1_squared_c = c.c * t_minus_1_squared;
    const bc_float_t t_2 = 2 * t;
    
    const float t_squared = powf(t,2);
    
    BCCubic2 out;
    out.a.xy = c.a;
    out.a.zw = powf(t,3) * c.b - 3 * t_squared * t_minus_1_d + 3 * t * t_minus_1_squared_c - powf(t_minus_1,3) * c.a;
    out.b.xy = out.a.zw;
    out.b.zw = c.b;
    
    out.c.xy = t_c - t_minus_1 * c.a;
    out.c.zw = t_squared * c.b  - t_2 * t_minus_1_d + t_minus_1_squared_c;
    
    out.d.xy = t_squared * c.d - t_2 * t_minus_1 * c.c + t_minus_1_squared * c.a;
    out.d.zw = t * c.b - t_minus_1_d;
    return out;
}

bc_float_t BCCubicArclengthParameterizationWithBounds(BCCubic cubic, bc_float_t length, bc_float_t lowerBound, bc_float_t upperBound, bc_float_t threshold) {
    while (true) {
        bc_float2_t upperEvaluate = BCCubicEvaluate(cubic, upperBound);
        simd_float2 lowerEvaluate = BCCubicEvaluate(cubic, lowerBound);
        if (simd_distance(upperEvaluate, lowerEvaluate) < threshold) {
            return lowerBound;
        }
        bc_float_t partition = (upperBound - lowerBound) / 2 + lowerBound;
        BCCubic d = BCCubicLeftSplit(cubic, partition);

        if (BCCubicLength(d) > length) { //choose left
            upperBound = partition;
        }
        else { //choose right
            lowerBound = partition;
        }
    }
}
void BCCubicNormalize(BCCubic __BC_DEVICE *c, bc_float_t approximateDistance) {
    __BC_ASSERT(approximateDistance > 0);
    if (simd_distance(c->c, c->a) < approximateDistance) {
        const BCLine asLine = BCCubicAsLine(*c);
        bc_float_t t = BCLineArclengthParameterization(asLine, approximateDistance);
        c->c = BCLineEvaluate(asLine,t);
    }
    if (simd_distance(c->d, c->b) < approximateDistance) {
        const BCLine asLine = BCCubicAsLine(*c);
        bc_float_t t = 1 - BCLineArclengthParameterization(asLine, approximateDistance);
        c->d = BCLineEvaluate(asLine,t);
    }
}

