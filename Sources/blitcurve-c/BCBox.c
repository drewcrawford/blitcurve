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
