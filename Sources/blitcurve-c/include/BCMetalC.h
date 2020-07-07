// BCMetalC.h: C-style syntax for metal
// ©2020 DrewCrawfordApps LLC

//#ifndef BCMetalC_h
//#define BCMetalC_h
//
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

#define fabsf fabs
#define sin(X) metal::sin(X)
#define cos(X) metal::cos(X)
#define atan2(X,Y) metal::atan2(X,Y)
#define pow(X,Y) metal::pow(X,Y)
#define fabs(X) metal::fabs(X)

//there is no simd_norm_inf in metal
#define simd_norm_inf(N) metal::fmax(fabs(N.x),fabs(N.y))

#define bc_make_4x2 metal::float4x2
#define bc_make_2x4 metal::float2x4
#define bc_make_4x4 metal::float4x4
#define simd_mul(X,Y) X * Y
#define simd_sub(X,Y) X - Y
#define simd_reduce_add(V4) V4.x + V4.y + V4.z + V4.w
#define simd_length simd::length
#define simd_abs(X) simd::abs(X)
#define simd_length_squared simd::length_squared
#define simd_distance simd::distance
#define simd_dot simd::dot

#define __BC_DEVICE device

#define BC_NO_VEC8 //simd8/16 are not available


#else //defines we need in C

#define bc_make_4x2 simd_matrix
#define bc_make_2x4 simd_matrix
#define bc_make_4x4 simd_matrix

//address space qualifier
#define __BC_DEVICE

#endif

//#endif
