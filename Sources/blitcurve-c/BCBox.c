//BCBox.c: BCBox functions
// ©2020 DrewCrawfordApps LLC

#include "BCBox.h"
bool BCBoxIsPointOnOrInside(BCBox b, bc_float2_t point) {
    simd_float2 ab = b.b - b.a;
    simd_float2 am = point - b.a;
    simd_float2 bc = b.c - b.b;
    simd_float2 bm = point - b.b;

    float abam = simd_dot(ab, am);
    float abab = simd_dot(ab, ab);
    float bcbm = simd_dot(bc, bm);
    float bcbc = simd_dot(bc, bc);

    if (0 <= abam && abam <= abab && 0 <= bcbm && bcbm <= bcbc) {
        return true;
    }
    return false;
}

BCBox BCBoxMakeWithCenterAndAngle(bc_float2_t center, bc_float_t angle, bc_float2_t lengths) {
    bc_float2_t halflengths = lengths / 2;
    bc_float2_t _a = simd_make_float2(-halflengths.x, -halflengths.y);
    bc_float2_t _b = simd_make_float2(-halflengths.x,halflengths.y);
    bc_float2_t _c = simd_make_float2(halflengths.x,halflengths.y);
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(angle),sin(angle)), simd_make_float2(-sin(angle),cos(angle)));
    bc_float4_t a_b = simd_make_float4(simd_mul(_a,magic),simd_mul(_b,magic));
    bc_float2_t c = simd_mul(_c,magic);
    a_b += simd_make_float4(center,center);
    c += center;
    BCBox out;
    out.a = a_b.xy;
    out.b = a_b.zw;
    out.c = c;
    return out;
}

