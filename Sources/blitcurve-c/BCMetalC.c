//BCMetalC.c: Cross-platform metal/C api
// ©2020 DrewCrawfordApps LLC
#import "BCMetalC.h"
//on metal, these are "normal inline"
#ifdef __METAL_VERSION__

extern inline bc_float2_t bc_make_float2(float a, float b);
extern inline bc_float3_t bc_make_float3(float a, float b, float c);
extern inline bc_float4_t bc_make_float4(float x, float y, float z, float w);
extern inline bc_float4_t bc_make_float4(bc_float2_t a, bc_float2_t b);
extern inline bc_float_t bc_reduce_min(bc_float2_t x);
extern inline bc_float_t bc_reduce_min(bc_float4_t x);
extern inline bc_float_t bc_reduce_max(bc_float2_t x);
extern inline bc_float_t bc_reduce_max(bc_float4_t x);


extern inline float bc_reduce_add(simd_float4 v);

extern inline float bc_reduce_add(simd_float2 v);
//in Metal, we get warnings about putting overloadable here
//in C, we get errors if we do not
#define MAYBE_OVERLOADABLE
#else
#define MAYBE_OVERLOADABLE __attribute__((overloadable))
#endif



MAYBE_OVERLOADABLE extern inline bc_float_t bc_pow(float a, float b);

MAYBE_OVERLOADABLE extern inline bc_float_t bc_cos(float a);

MAYBE_OVERLOADABLE extern inline bc_float_t bc_sin(float a);

MAYBE_OVERLOADABLE extern inline bc_float_t bc_abs(float a);
