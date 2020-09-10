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

