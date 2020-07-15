//BCRect.c: BCRect functions
// ©2020 DrewCrawfordApps LLC

#include "BCRect.h"
bool BCRectIsPointOnOrInside(BC3Points b, bc_float2_t point) {
    simd_float2 ab = b.a_b.hi - b.a_b.lo;
    simd_float2 am = point - b.a_b.lo;
    simd_float2 bc = b.c - b.a_b.hi;
    simd_float2 bm = point - b.a_b.hi;

    float abam = simd_dot(ab, am);
    float abab = simd_dot(ab, ab);
    float bcbm = simd_dot(bc, bm);
    float bcbc = simd_dot(bc, bc);

    if (0 <= abam && abam <= abab && 0 <= bcbm && bcbm <= bcbc) {
        return true;
    }
    return false;
}

BC4Points BCRectGet4Points(BCRect r) {
    bc_float2_t halflengths = r.lengths / 2;
    //unclear to me why, but we need to flip y and x
    bc_float2_t halflengths_t = simd_make_float2(halflengths.y,halflengths.x);
    bc_float2_t _a = simd_make_float2(halflengths_t.x, -halflengths_t.y);
    bc_float2_t _b = simd_make_float2(halflengths_t.x,halflengths_t.y);
    bc_float2_t _c = simd_make_float2(-halflengths_t.x,halflengths_t.y);
    bc_float2_t _d = simd_make_float2(-halflengths_t.x, -halflengths_t.y);
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(r.angle),sin(r.angle)), simd_make_float2(-sin(r.angle),cos(r.angle)));
    bc_float4_t a_b = simd_make_float4(simd_mul(_a,magic),simd_mul(_b,magic));
    bc_float4_t c_d = simd_make_float4(simd_mul(_c, magic),simd_mul(_d,magic));
    a_b += simd_make_float4(r.center,r.center);
    c_d += simd_make_float4(r.center,r.center);
    BC4Points out;
    out.a_b = a_b;
    out.c_d = c_d;
    return out;
}

BC3Points BCRectGet3Points(BCRect r) {
    bc_float2_t halflengths = r.lengths / 2;
    //unclear to me why, but we need to flip y and x
    bc_float2_t halflengths_t = simd_make_float2(halflengths.y,halflengths.x);
    bc_float2_t _a = simd_make_float2(halflengths_t.x, -halflengths_t.y);
    bc_float2_t _b = simd_make_float2(halflengths_t.x,halflengths_t.y);
    bc_float2_t _c = simd_make_float2(-halflengths_t.x,halflengths_t.y);
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(r.angle),sin(r.angle)), simd_make_float2(-sin(r.angle),cos(r.angle)));
    bc_float4_t a_b = simd_make_float4(simd_mul(_a,magic),simd_mul(_b,magic));
    bc_float2_t c = simd_mul(_c, magic);
    a_b += simd_make_float4(r.center,r.center);
    c += r.center;
    BC3Points out;
    out.a_b = a_b;
    out.c = c;
    return out;
}

