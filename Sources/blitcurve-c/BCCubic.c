// BCCubic.c: Cubic implementation
// ©2020 DrewCrawfordApps LLC

#include "BCCubic.h"

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
    const bc_float_t t_minus_1_squared = pow(t_minus_1,2);
    const bc_float2_t t_squared_d = pow(t,2) * c.d;
    const bc_float2_t t_c = t * c.c;
    out.a = c.a;
    out.b = pow(t,3) * c.b - 3 * t_squared_d * t_minus_1 + 3 * t_minus_1_squared * t_c - pow(t_minus_1,3) * c.a;
    out.c = t_c - t_minus_1 * c.a;
    out.d = t_squared_d - 2 * t_minus_1 * t_c + t_minus_1_squared * c.a;
    return out;
}

BCCubic2 BCCubicSplit(BCCubic c, bc_float_t t) {
    const bc_float_t t_minus_1 = t - 1;
    const bc_float2_t t_minus_1_d = t_minus_1 * c.d;
    const bc_float2_t t_c = t * c.c;
    const bc_float_t t_minus_1_squared = pow(t_minus_1, 2);
    const bc_float2_t t_minus_1_squared_c = c.c * t_minus_1_squared;
    const bc_float_t t_2 = 2 * t;
    
    const float t_squared = pow(t,2);
    
    BCCubic2 out;
    out.a.xy = c.a;
    out.a.zw = pow(t,3) * c.b - 3 * t_squared * t_minus_1_d + 3 * t * t_minus_1_squared_c - pow(t_minus_1,3) * c.a;
    out.b.xy = out.a.zw;
    out.b.zw = c.b;
    
    out.c.xy = t_c - t_minus_1 * c.a;
    out.c.zw = t_squared * c.b  - t_2 * t_minus_1_d + t_minus_1_squared_c;
    
    out.d.xy = t_squared * c.d - t_2 * t_minus_1 * c.c + t_minus_1_squared * c.a;
    out.d.zw = t * c.b - t_minus_1_d;
    return out;
}

