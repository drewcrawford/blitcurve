//BCCubicDrawing.c: Drawing functions for BCCubic
// ©2020 DrewCrawfordApps LLC



#include "BCCubicDrawing.h"
#include "BCMetalC.h"
#include "BCBezierParameter.h"
__attribute__((overloadable))
extern inline simd_float3 BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, simd_float3x3 transform);


__attribute__((const))
__attribute__((overloadable))
simd_float3 BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, simd_float3x3 transform, bc_float_t minimum, bc_float_t maximum) {
    __BC_ASSERT(minimum < maximum);
    __BC_ASSERT(vertexID < vertexesPerCubic);
    const bc_float_t parameter = BCVertexToBezierParameter(vertexID, vertexesPerCubic, minimum, maximum);
    simd_float3 output = simd_make_float3(0,0,1);
    output.xy = BCCubicEvaluate(cubic, parameter);
    output.xyz = simd_mul(output.xyz, transform);
    return output;
}

__attribute__((const))
__attribute__((overloadable))
simd_float3 BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, simd_float3x3 transform, bc_float_t minimum, bc_float_t maximum, bc_float_t minimumDeltaT) {
    __BC_ASSERT(minimum <= maximum);
    if (maximum - minimum < minimumDeltaT) {
        minimum = simd_max(0.0f,minimum - minimumDeltaT);
        maximum = simd_min(1.0f,maximum + minimumDeltaT);
    }
    return BCCubicVertexMake(cubic, vertexID, vertexesPerCubic, transform, minimum, maximum);
}
