// BCMetalC.h: Compatibility layer for C/Metal portability
// ©2020 DrewCrawfordApps LLC

#ifndef BCMetalC_h
#define BCMetalC_h
#include "BCTypes.h"

#ifdef __METAL_VERSION__
#include <metal_stdlib>
inline bc_float2_t bc_make_float2(float a, float b) {
    return simd::float2(a,b);
}
inline bc_float3_t bc_make_float3(float a, float b, float c) {
    return simd::float3(a,b,c);
}
inline bc_float4_t bc_make_float4(float x, float y, float z, float w) {
    return simd::float4(x,y,z,w);
}
inline bc_float4_t bc_make_float4(simd::float2 a, simd::float2 b) {
    return simd::float4(a,b);
}
inline bc_float_t bc_reduce_min(bc_float2_t x) {
    return metal::min(x.x,x.y);
}
inline bc_float_t bc_reduce_min(bc_float4_t x) {
    return metal::min(metal::min(metal::min(x.x,x.y),x.w),x.z);
}

inline bc_float_t bc_reduce_max(bc_float2_t x) {
    return metal::max(x.x,x.y);
}

inline bc_float_t bc_reduce_max(simd::float4 x) {
    return metal::max(metal::max(metal::max(x.x,x.y),x.w),x.z);
}


#define bc_fast_length metal::fast::length
#define bc_mix simd::mix



#define BC_M_PI_F M_PI_F
#define BC_M_PI_2_F M_PI_2_F

#define bc_fabsf metal::fabs
#define bc_sinf metal::sin
#define bc_cosf metal::cos
#define bc_sqrt(X) metal::sqrt(X)
#define bc_pow metal::pow
#define bc_cos metal::cos
#define bc_sin metal::sin

#define bc_powf metal::pow
#define bc_signbit metal::signbit
#define bc_atan2 metal::atan2
#define bc_round metal::round
#define bc_ceil metal::ceil
#define bc_floor metal::floor
#define bc_atan metal::atan

#define bc_norm_inf(N) metal::fmax(metal::abs(N.x),metal::abs(N.y))

#define bc_make_4x2 metal::float4x2
#define bc_make_2x2 metal::float2x2
#define bc_make_2x4 metal::float2x4
#define bc_make_4x4 metal::float4x4
#define bc_mul(X,Y) X * Y
#define bc_sub(X,Y) X - Y
__attribute__((overloadable))
inline float bc_reduce_add(simd_float4 v) {
    return v.x + v.y + v.z + v.w;
}
__attribute__((overloadable))
inline float bc_reduce_add(simd_float2 v) {
    return v.x + v.y;
}

#define bc_length simd::length
#define bc_abs simd::abs
#define bc_length_squared simd::length_squared
#define bc_distance simd::distance
#define bc_dot simd::dot
#define bc_max simd::max
#define bc_min simd::min
#define bc_sign simd::sign


#define __BC_DEVICE device

#define BC_NO_VEC8 //simd8/16 are not available


#else //defines we need in C


#define bc_make_4x2 simd_matrix
#define bc_make_2x4 simd_matrix
#define bc_make_4x4 simd_matrix
#define bc_make_2x2 simd_matrix

#define BC_M_PI_F M_PI



__attribute__((overloadable))
inline bc_float_t bc_pow(float a, float b) { return powf(a,b); }
static inline bc_float2_t bc_pow(bc_float2_t a, bc_float2_t b) { return pow(a,b); }
__attribute__((overloadable))
inline bc_float_t bc_cos(float a) { return cosf(a); }
static inline bc_float2_t bc_cos(bc_float2_t a) { return cos(a); }
__attribute__((overloadable))
inline bc_float_t bc_sin(float a) { return sinf(a); }
static inline bc_float2_t bc_sin(bc_float2_t a) { return sin(a); }
__attribute__((overloadable))
inline bc_float_t bc_abs(float a) { return fabsf(a);}
__attribute__((overloadable))
static inline bc_float2_t bc_abs(bc_float2_t a) { return simd_abs(a);}

#define bc_reduce_max simd_reduce_max
#define bc_max fmax
#define bc_min fmin
#define bc_dot simd_dot
#define bc_norm_inf simd_norm_inf
#define bc_fast_length simd_fast_length
#define bc_atan2 atan2f
#define bc_mix simd_mix
#define bc_distance simd_distance
#define bc_length simd_length
#define bc_round roundf
#define bc_ceil ceilf
#define bc_floor floorf
#define bc_mul simd_mul
#define bc_sub simd_sub
#define bc_reduce_add simd_reduce_add
#define bc_reduce_min simd_reduce_min
#define bc_length_squared simd_length_squared
#define bc_sqrt sqrtf
#define bc_atan atan
#define bc_signbit signbit




__attribute__((overloadable))
static inline bc_float4_t bc_make_float4(float x, float y, float z, float w) {
    return simd_make_float4(x,y,z,w);
}
static inline bc_float4_t bc_make_float4(bc_float2_t a, bc_float2_t b) {
    return simd_make_float4(a,b);
}
#define bc_make_float2 simd_make_float2
#define bc_make_float3 simd_make_float3

//address space qualifier
#define __BC_DEVICE
#endif

#endif //BCMetalC_h
