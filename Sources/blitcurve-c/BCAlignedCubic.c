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
    const bc_float_t p5 = 3 * c.c.x * p4;
    const bc_float_t p6 = -6 * c.c.x * p2_t;
    const bc_float_t p7 = 6 * c.d.x * p2_t;
    const bc_float_t p8 = 3 * c.b_x * p3;
    const bc_float_t p9 = -3 * c.d.x * p3;
    const bc_float_t p10 = p5 + p6 + p7 + p8 + p9;
    const bc_float_t p11 = 3 * c.c.y * p4;
    const bc_float_t p12 = -6 * c.c.y * p2_t;
    const bc_float_t p13 = 6 * c.d.y * p2_t;
    const bc_float_t p14 = -3 * c.d.y * p3;
    const bc_float_t p15 = p11 + p12 + p13 + p14;
    
    const bc_float_t n1 = -12 * c.c.y * p2 + 6 * c.d.y * p2 + 6 * c.c.y * t - 12 * c.d.y * t * p10;
    const bc_float_t n2 = -12 * c.c.x * p2 + 6 * c.d.x * p2 + 6 * c.b_x * t + 6 * c.c.x * t - 12 * c.d.x * t * p15;
    
    const bc_float_t d1 = p10 * p10 + p15 * p15;
    const bc_float_t d2 = powf(d1, 3.0/2.0);
    
    return (n1 - n2) / d2;
}
