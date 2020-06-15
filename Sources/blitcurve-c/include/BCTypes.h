// Types.h: Various types
// ©2020 DrewCrawfordApps LLC

#ifndef Types_h
#define Types_h

#ifndef __METAL_VERSION__
#import <simd/simd.h>
#endif
__attribute__((swift_name("BCFloat")))
typedef float bc_float_t;

__attribute__((swift_name("BCFloat2")))
#ifdef __METAL_VERSION__
typedef simd::float2 bc_float2_t;
#else
typedef simd_float2 bc_float2_t;
#endif

__attribute__((swift_name("BCFloat4")))
#ifdef __METAL_VERSION__
typedef simd::float4 bc_float4_t;
#else
typedef simd_float4 bc_float4_t;
#endif

#endif
