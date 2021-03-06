//BCCubicDrawing.c: Drawing functions for BCCubic
// ©2020-2021 DrewCrawfordApps LLC



#include "BCCubicDrawing.h"
#include "BCMetalC.h"
#include "BCBezierParameter.h"
__attribute__((overloadable))
extern inline bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform);

__attribute__((const))
__attribute__((overloadable))
bc_float2_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t minimum, bc_float_t maximum) {
    __BC_PRECONDITION(minimum < maximum,BCVertex2ErrorMake(BCErrorArgRelationship));
    __BC_RANGEASSERT(vertexID < vertexesPerCubic, BCVertex2ErrorMake(BCErrorArgRelationship));
    const bc_float_t parameter = BCVertexToBezierParameter(vertexID, vertexesPerCubic, minimum, maximum);
    __BC_TRY_IF(parameter < 0, BCVertex2ErrorMake((BCError)(-1 * ((int)parameter + 1))));
    return BCCubicEvaluate(cubic, parameter);
}

__attribute__((const))
__attribute__((overloadable))
bc_float2_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t minimum, bc_float_t maximum, bc_float_t minimumDeltaT) {
    __BC_PRECONDITION(minimum <= maximum,BCVertex2ErrorMake(BCErrorArgRelationship));
    if (maximum - minimum < minimumDeltaT) {
        minimum = bc_max(0.0f,minimum - minimumDeltaT);
        maximum = bc_min(1.0f,maximum + minimumDeltaT);
    }
    //rvalue should be compatible; passthrough
    return BCCubicVertexMake(cubic, vertexID, vertexesPerCubic, minimum, maximum);
}

bc_float2_t BCCubicVertexMakeClampedParameterization(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t startPosition, const bc_float_t endPosition, bc_float_t threshold,bc_float_t minimumDelta) {
    const bc_float_t minLinearPosition = bc_max(0.0,startPosition);
    const bc_float_t maxLinearPosition = bc_min(BCCubicLength(cubic),endPosition);

    const bc_float_t minT = BCCubicArclengthParameterization(cubic, minLinearPosition,threshold);
    __BC_TRY_IF(minT<0,BCVertex2ErrorMake(BCErrorLogic));
    const bc_float_t maxT = BCCubicArclengthParameterization(cubic, maxLinearPosition, threshold);
    __BC_TRY_IF(maxT<0,BCVertex2ErrorMake(BCErrorLogic));
    //rvalue is compatible here, so we should be able to passthrough
    return BCCubicVertexMake(cubic, vertexID, vertexesPerCubic, minT, maxT, minimumDelta);
}

