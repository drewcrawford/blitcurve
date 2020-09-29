// Types.h: Various types
// ©2020 DrewCrawfordApps LLC

#ifndef Types_h
#define Types_h

#ifndef __METAL_VERSION__
#include <simd/simd.h>
#else
#include <metal_stdlib>
#endif
__attribute__((swift_name("BCFloat")))
///@typedef BlitCurve's internal float type.
typedef float bc_float_t;

__attribute__((swift_name("BCFloat2")))
///@typedef BlitCurve's internal float2 type.
#ifdef __METAL_VERSION__
typedef simd::float2 bc_float2_t;
#else
typedef simd_float2 bc_float2_t;
#endif

__attribute__((swift_name("BCFloat3")))
///@typedef BlitCurve's internal float3 type.
#ifdef __METAL_VERSION__
typedef simd::float3 bc_float3_t;
#else
typedef simd_float3 bc_float3_t;
#endif

__attribute__((swift_name("BCFloat4")))
///@typedef BlitCurve's internal float4 type.
#ifdef __METAL_VERSION__
typedef simd::float4 bc_float4_t;
#else
typedef simd_float4 bc_float4_t;
#endif

__attribute__((swift_name("BCFloat2x2")))
///@typedef BlitCurve's internal float2 type.
#ifdef __METAL_VERSION__
typedef metal::float2x2 bc_float2x2_t;
#else
typedef simd_float2x2 bc_float2x2_t;
#endif

__attribute__((swift_name("BCFloat3x3")))
///@typedef BlitCurve's internal float2 type.
#ifdef __METAL_VERSION__
typedef metal::float3x3 bc_float3x3_t;
#else
typedef simd_float3x3 bc_float3x3_t;
#endif

__attribute__((swift_name("BCFloat4x4")))
///@typedef BlitCurve's internal float2 type.
#ifdef __METAL_VERSION__
typedef metal::float4x4 bc_float4x4_t;
#else
typedef simd_float4x4 bc_float4x4_t;
#endif


#endif
