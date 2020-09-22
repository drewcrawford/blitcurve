// BCMetalC.h: C-style syntax for metal
// ©2020 DrewCrawfordApps LLC

#ifndef BCMetalC_h
#define BCMetalC_h

#ifdef __METAL_VERSION__
#include <metal_stdlib>
//__attribute__((__overloadable__))
static inline simd::float2 simd_make_float2(float a, float b) {
    return simd::float2(a,b);
}
////__attribute__((__overloadable__))
static inline simd::float4 simd_make_float4(float x, float y, float z, float w) {
    return simd::float4(x,y,z,w);
}
//__attribute__((__overloadable__))
static inline simd::float4 simd_make_float4(simd::float2 a, simd::float2 b) {
    return simd::float4(a,b);
}
static inline float simd_reduce_min(simd::float2 x) {
    return metal::min(x.x,x.y);
}
static inline float simd_reduce_min(simd::float4 x) {
    return metal::min(metal::min(metal::min(x.x,x.y),x.w),x.z);
}

static inline float simd_reduce_max(simd::float2 x) {
    return metal::max(x.x,x.y);
}

static inline float simd_reduce_max(simd::float4 x) {
    return metal::max(metal::max(metal::max(x.x,x.y),x.w),x.z);
}


#define simd_fast_length(X) metal::fast::length(X)
#define simd_mix(X,Y,Z) simd::mix(X,Y,Z)



#define M_PI M_PI_F
#define M_PI_2 M_PI_2_F

#define fabsf metal::fabs
#define sinf metal::sin
#define cosf metal::cos
#define sqrtf(X) metal::sqrt((float) X)
//powf is used for pure floats, and simd_powf for simd types
//in metal these are identical, but not so in c
#define simd_powf metal::pow
#define simd_cosf metal::cos
#define simd_sinf metal::sin

#define powf metal::pow
#define signbit metal::signbit
#define atan2(X,Y) metal::atan2(X,Y)
#define roundf metal::round
#define ceilf metal::ceil
#define floorf metal::floor
#define atan metal::atan

//there is no simd_norm_inf in metal
#define simd_norm_inf(N) metal::fmax(fabsf(N.x),fabsf(N.y))

#define bc_make_4x2 metal::float4x2
#define bc_make_2x2 metal::float2x2
#define bc_make_2x4 metal::float2x4
#define bc_make_4x4 metal::float4x4
#define simd_mul(X,Y) X * Y
#define simd_sub(X,Y) X - Y
__attribute__((overloadable))
static inline float simd_reduce_add(simd_float4 v) {
    return v.x + v.y + v.z + v.w;
}
__attribute__((overloadable))
static inline float simd_reduce_add(simd_float2 v) {
    return v.x + v.y;
}

#define simd_length simd::length
#define simd_abs(X) simd::abs(X)
#define simd_length_squared simd::length_squared
#define simd_distance simd::distance
#define simd_dot simd::dot
#define simd_max simd::max
#define simd_sign simd::sign


#define __BC_DEVICE device

#define BC_NO_VEC8 //simd8/16 are not available


#else //defines we need in C

#define bc_make_4x2 simd_matrix
#define bc_make_2x4 simd_matrix
#define bc_make_4x4 simd_matrix
#define bc_make_2x2 simd_matrix

//in C, we want pow for simd types, not powf
#define simd_powf pow
#define simd_cosf cos
#define simd_sinf sin

//address space qualifier
#define __BC_DEVICE
#endif

#endif //BCMetalC_h
