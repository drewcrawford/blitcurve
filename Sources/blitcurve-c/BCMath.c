// BCMath.c: Math
// ©2020 DrewCrawfordApps LLC

#include "BCMath.h"

__attribute__((const))
bool BCIsNearlyEqual(bc_float_t a, bc_float_t b) {
    return (fabs(a - b) < 0.001);
}

__attribute__((const))
bool BCIsNearlyEqual2(bc_float2_t a, bc_float2_t b) {
    return BCIsNearlyEqual(a.x, b.x) && BCIsNearlyEqual(a.y, b.y);
}
