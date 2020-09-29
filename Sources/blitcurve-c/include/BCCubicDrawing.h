//BCCubicDrawing.h: Drawing code for BCCubic
// ©2020 DrewCrawfordApps LLC

#ifndef BCCubicDrawing_h
#define BCCubicDrawing_h

#include "BCCubic.h"



/**
\abstract Creates a 3D vertex suitable for drawing a portion of a cubic.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param transform A projection matrix to apply to the output vertex
 \param minimum the minimum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param maximum the maximum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 */
__attribute__((const))
__attribute__((overloadable))
bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform, bc_float_t minimum, bc_float_t maximum);

/**
\abstract Creates a 3D vertex suitable for drawing a portion of a cubic, making sure the portion has a minimum size.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param transform A projection matrix to apply to the output vertex
 \param minimum the minimum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param maximum the maximum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param minimumDeltaT the minimum range to be drawn.  If \c maximum-minimum is less than this, the range will be adjusted.
 */
__attribute__((const))
__attribute__((overloadable))
bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform, bc_float_t minimum, bc_float_t maximum, bc_float_t minimumDeltaT);

/**
\abstract Creates a 3D vertex suitable for drawing an entire cubic.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param transform A projection matrix to apply to the output vertex
 */
__attribute__((const))
__attribute__((overloadable))
inline bc_float3_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float3x3_t transform) {
    return BCCubicVertexMake(cubic,vertexID, vertexesPerCubic, transform, 0,1);
}

#endif
