// Math.h: Math functions
// ©2020 DrewCrawfordApps LLC

#ifndef Math_h
#define Math_h
#include "BCMetalC.h"
#ifndef __METAL_VERSION__
#include <stdbool.h>
#include <math.h>
#endif

#include "BCTypes.h"

__attribute__((const))
__attribute__((swift_name("IsNearlyEqual(a:b:)")))
///Deteremines if the points are nearly equal
static inline bool BCIsNearlyEqual(bc_float_t a, bc_float_t b) {
    return (bc_abs(a - b) < 0.001);
}

__attribute__((const))
__attribute__((swift_name("IsNearlyEqual(a:b:)")))
///Deteremines if the points are nearly equal
static inline bool BCIsNearlyEqual2(bc_float2_t a, bc_float2_t b) {
    bc_float2_t diff = a - b;
    
    return bc_norm_inf(diff) < 0.001;
}

static inline float __bc_cube(float a) {
    //metal::pow ought to be avoided, FB8904929
    return a * a * a;
}
__attribute__((overloadable))
static inline float __bc_square(float a) {
    //metal::pow ought to be avoided, FB8904929
    return a * a;
}

static inline bc_float2_t __bc_square(bc_float2_t a) {
    //metal::pow ought to be avoided, FB8904929
    return a * a;
}
#endif
