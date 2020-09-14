//BCAlignedCubic.c: Aligned cubic definition
// ©2020 DrewCrawfordApps LLC



#include "BCAlignedCubic.h"
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
    r.b_x = t_b.x * cos(theta) - t_b.y * sin(theta);
    //and then a standard rotation for c,d
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(theta),sin(theta)), simd_make_float2(-sin(theta),cos(theta)));
    r.c = simd_mul(magic,t_c);
    r.d = simd_mul(magic,t_d);
    return r;
}


bc_float_t BCAlignedCubicKappa(BCAlignedCubic c, bc_float_t t) {
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
    
    const bc_float2_t n2pre_n1 = -12 * p2by.lo + 6 * p2by.hi + 6 * tby.lo - 12 * tby.hi * simd_make_float2(p10pre_p15.y,p10);
    const bc_float_t n2 = n2pre_n1.x + 6 * c.b_x * t;
    
    const bc_float_t d1 = p10 * p10 + p10pre_p15.y * p10pre_p15.y; //simd-length may be slow here
    const bc_float_t d2 = powf(d1, 3.0/2.0);
    
    return (n2pre_n1.y - n2) / d2;
}
