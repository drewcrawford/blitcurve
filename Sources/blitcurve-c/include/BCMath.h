// Math.h: Math functions
// ©2020 DrewCrawfordApps LLC

#ifndef Math_h
#define Math_h
#import <stdbool.h>
#import "BCTypes.h"

__attribute__((const))
__attribute__((swift_name("IsNearlyEqual(a:b:)")))
static inline bool BCIsNearlyEqual(bc_float_t a, bc_float_t b) {
    return (fabs(a - b) < 0.001);
}

__attribute__((const))
__attribute__((swift_name("IsNearlyEqual(a:b:)")))
inline bool BCIsNearlyEqual2(bc_float2_t a, bc_float2_t b) {
    return BCIsNearlyEqual(a.x, b.x) && BCIsNearlyEqual(a.y, b.y);
}
#endif
