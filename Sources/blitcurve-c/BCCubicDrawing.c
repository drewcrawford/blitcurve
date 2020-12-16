//BCCubicDrawing.c: Drawing functions for BCCubic
// ©2020 DrewCrawfordApps LLC



#include "BCCubicDrawing.h"
#include "BCMetalC.h"
#include "BCBezierParameter.h"
__attribute__((overloadable))
extern inline bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform);


__attribute__((const))
__attribute__((overloadable))
bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform, bc_float_t minimum, bc_float_t maximum) {
    __BC_PRECONDITION(minimum < maximum,BCVertex3ErrorMake(BCErrorArgRelationship));
    __BC_RANGEASSERT(vertexID < vertexesPerCubic, BCVertex3ErrorMake(BCErrorArgRelationship));
    const bc_float_t parameter = BCVertexToBezierParameter(vertexID, vertexesPerCubic, minimum, maximum);
    __BC_PRECONDITION_CONVERT(parameter < 0, BCVertex3ErrorMake(-1 * ((int)parameter + 1)));
    bc_float3_t output = bc_make_float3(0,0,1);
    output.xy = BCCubicEvaluate(cubic, parameter);
    output.xyz = bc_mul(output.xyz, transform);
    return output;
}

__attribute__((const))
__attribute__((overloadable))
bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform, bc_float_t minimum, bc_float_t maximum, bc_float_t minimumDeltaT) {
    __BC_PRECONDITION(minimum <= maximum,BCErrorArgRelationship);
    if (maximum - minimum < minimumDeltaT) {
        minimum = bc_max(0.0f,minimum - minimumDeltaT);
        maximum = bc_min(1.0f,maximum + minimumDeltaT);
    }
    return BCCubicVertexMake(cubic, vertexID, vertexesPerCubic, transform, minimum, maximum);
}

bc_float4_t BCCubicVertexMakeClampedParameterization(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform, bc_float_t startPosition, bc_float_t endPosition, bc_float_t threshold,bc_float_t minimumDelta) {
    bc_float_t minLinearPosition = bc_max(0.0,startPosition);
    bc_float_t maxLinearPosition = bc_min(BCCubicLength(cubic),endPosition);
    
    bc_float_t minT = BCCubicArclengthParameterization(cubic, minLinearPosition,threshold);
    bc_float_t maxT = BCCubicArclengthParameterization(cubic, maxLinearPosition, threshold);
    return BCCubicVertexMakeClip(cubic, vertexID, vertexesPerCubic, transform, minT, maxT, minimumDelta);
}
