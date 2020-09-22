//BCAlignedCubic.c: Aligned cubic definition
// ©2020 DrewCrawfordApps LLC



#include "BCAlignedCubic.h"
extern inline bc_float_t BCAlignedCubicCurveRadius(BCAlignedCubic c, bc_float_t t);

BCAlignedCubic BCAlignedCubicMake(BCCubic c) {
    //first we translate by the -a
    //this will place a at 0,0
    const bc_float2_t t_c = c.c - c.a;
    const bc_float2_t t_d  = c.d - c.a;
    const bc_float2_t t_b = c.b - c.a;
    //now we need to find a rotation angle to get by==0.
    const float theta = -atan(t_b.y/t_b.x);
    //with that solved, we can apply the rotation to get b_x
    BCAlignedCubic r;
    r.b_x = t_b.x * cosf(theta) - t_b.y * sinf(theta);
    //and then a standard rotation for c,d
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cosf(theta),sinf(theta)), simd_make_float2(-sinf(theta),cosf(theta)));
    r.c = simd_mul(magic,t_c);
    r.d = simd_mul(magic,t_d);
    return r;
}


bc_float_t BCAlignedCubicKappa(BCAlignedCubic c, bc_float_t t) {
    __BC_ASSERT(t >=0 && t <= 1);
    const bc_float_t p1 = -t;
    const bc_float_t p2 = 1 + p1;
    const bc_float_t p2_t = p2 * t;
    const bc_float_t p3 = t * t;
    const bc_float_t p4 = p2 * p2;
    const bc_float2_t p5_p11 = 3 * c.c * p4;
    const bc_float2_t p6_p12 = -6 * c.c * p2_t;
    const bc_float2_t p7_p13 = 6 * c.d * p2_t;
    const bc_float_t p8 = 3 * c.b_x * p3;
    const bc_float2_t p9_p14 = -3 * c.d * p3;
    
    const bc_float2_t p10pre_p15 = p5_p11 + p6_p12 + p7_p13 + p9_p14;
    const bc_float_t p10 = p10pre_p15.x + p8;
    
    const bc_float4_t p2by = simd_make_float4(c.c,c.d) * p2;
    const bc_float4_t tby = simd_make_float4(c.c,c.d) * t;
    
    //const bc_float2_t
    bc_float2_t n2_n1 = -12 * p2by.lo + 6 * p2by.hi + 6 * tby.lo - 12 * tby.hi;
    
    n2_n1.x += 6 * c.b_x * t;
    n2_n1 *= simd_make_float2(p10pre_p15.y,p10);
    
    const bc_float_t d1 = p10 * p10 + p10pre_p15.y * p10pre_p15.y; //simd-length may be slow here
    const bc_float_t d2 = powf(d1, 3.0/2.0);
    
    return (n2_n1.y - n2_n1.x) / d2;
}

__BC_MAYBESTATIC bc_float_t __BCAlignedCubicKappaPrime(BCAlignedCubic c, bc_float_t t) {
    __BC_ASSERT(t >= 0 && t <= 1);
    const bc_float2_t c3 = c.c * 3;
    const bc_float_t p1 = -t;
    const bc_float_t p2 = 1 + p1;
    const bc_float4_t cd_p2 = simd_make_float4(c.c,c.d) * p2;
    const bc_float4_t cd_p2_t = cd_p2 * t;
    const bc_float2_t p3_p4 = simd_make_float2(t,p2) * simd_make_float2(t,p2);
    const bc_float2_t p5_p17 = -12 * cd_p2.xy;
    const bc_float2_t p6_p18 = 6 * cd_p2.zw;
    const bc_float_t p7 = 6 * c.b_x * t;
    const bc_float2_t p8_p19 = 6 * c.c * t;
    const bc_float2_t p9_p20 = -12 * c.d * t;
    bc_float2_t p10_p21 = p5_p17 + p6_p18 + p8_p19 + p9_p20;
    p10_p21.x += p7;
    const bc_float2_t p11_p22 = c3 * p3_p4.y;
    const bc_float2_t p12_p23 = -6 * cd_p2_t.xy;
    const bc_float2_t p13_p24 = 6 * cd_p2_t.zw;
    const bc_float_t p14 = 3 * c.b_x * p3_p4.x;
    const bc_float2_t p15_p25 = -3 * c.d * p3_p4.x;
    bc_float2_t p16_p26 = p11_p22 + p12_p23 + p13_p24 + p15_p25;
    p16_p26.x += p14;
    const bc_float2_t p27_p28 = p16_p26 * p16_p26;
    const bc_float_t p29 = simd_reduce_add(p27_p28);
    
    /*
     x  y   z  w
     10 21 10 21
   x 16 26 26 16
     -----------
     
     */
    const bc_float4_t p10_p21_by_p16_p26 = simd_make_float4(p10_p21, p10_p21) *  simd_make_float4(p16_p26, simd_make_float2(p16_p26.y, p16_p26.x));
    
    const bc_float_t n1_1 = -3 * (p10_p21_by_p16_p26.w - p10_p21_by_p16_p26.z);
    const bc_float_t n1_2 = 2 * p10_p21_by_p16_p26.x + 2 * p10_p21_by_p16_p26.y;
    const bc_float_t n2_1 = (c.c.y - c.d.y) * p16_p26.x * 18;
    const bc_float_t n2_2 = (c.b_x + c3.x - 3 * c.d.x) * 6 * p16_p26.y;
    bc_float2_t d = simd_powf(simd_make_float2(p29,p29), simd_make_float2(5.0/2.0,3.0/2.0));
    d.x *= 2;
    const bc_float2_t n = simd_make_float2(n1_1 * n1_2,n2_1-n2_2);
    return simd_reduce_add(n/d);
}

static bc_float_t KappaSearch(BCAlignedCubic c, bc_float_t lower, bc_float_t upper, bc_float_t accuracy) {
    __BC_ASSERT(upper >= lower);
    while (upper - lower > accuracy) {
        bc_float_t lowerPrime = __BCAlignedCubicKappaPrime(c, lower);
        bc_float_t upperPrime = __BCAlignedCubicKappaPrime(c, upper);
        if (signbit(lowerPrime) == signbit(upperPrime)) {
            return MAXFLOAT;
        }
        const bc_float_t midpoint = (upper - lower) / 2 + lower;
        const bc_float_t midPrime = __BCAlignedCubicKappaPrime(c, midpoint);
        if (signbit(midPrime) == signbit(upperPrime)) {
            upper = midpoint;
        }
        else {
            lower = midpoint;
        }
    }
    return lower;
}

bc_float_t BCAlignedCubicMaxKappaParameter(BCAlignedCubic c,bc_float_t accuracy) {
    float maxKappa = 0;
    float param = MAXFLOAT;
    for(unsigned char i_t = 0; i_t < 5; i_t++) {
        bc_float_t l = i_t * 0.2;
        bc_float_t r = (i_t + 1) * 0.2;
        const bc_float_t itry = KappaSearch(c, l, r, accuracy);
        if (itry != MAXFLOAT) {
            const bc_float_t proposedKappa = fabsf(BCAlignedCubicKappa(c, itry));
            if (proposedKappa > maxKappa) {
                maxKappa = proposedKappa;
                param = itry;
            }
        }
    }
    if (param == MAXFLOAT) {
        return 0;
    }
    return param;
}
