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



#define simd_fast_length(X) metal::fast::length(X)
#define simd_mix(X,Y,Z) simd::mix(X,Y,Z)



#define M_PI M_PI_F
#define M_PI_2 M_PI_2_F

#define sin(X) metal::sin(X)
#define cos(X) metal::cos(X)
#define atan2(X,Y) metal::atan2(X,Y)
#define pow(X,Y) metal::pow(X,Y)
#define fabs(X) metal::fabs(X)
//there is no simd_norm_inf in metal
#define simd_norm_inf(N) metal::fmax(fabs(N.x),fabs(N.y))

#define __BC_DEVICE device

#else //defines we need in C

//address space qualifier
#define __BC_DEVICE

#endif

//#endif
