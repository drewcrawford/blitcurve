//BCCubicDrawing.h: Drawing code for BCCubic
// ©2020 DrewCrawfordApps LLC

#ifndef BCCubicDrawing_h
#define BCCubicDrawing_h

#include "BCCubic.h"

/**Creates an "error vertex".  The first component is \c BC_FLOAT_LARGE and the last is \c BCError.*/
static inline bc_float2_t BCVertex2ErrorMake(BCError e) {
    bc_float2_t b;
    b.x = BC_FLOAT_LARGE;
    b.y = e;
    return b;
}

/**
 Determines if a given vertex is an error.
 \see \c BCVertex2ErrorMake
 */
static inline bool BCVertexIsError(bc_float2_t vtx) {
    return vtx.x == BC_FLOAT_LARGE;
}


/**
\abstract Creates a 2D vertex suitable for drawing a portion of a cubic.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param transform A projection matrix to apply to the output vertex
 \param minimum the minimum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param maximum the maximum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \returns a 2d coordinate for the vertex
 \throws Checks arguments with assert.  rvalue is \c BCVertex2ErrorMake
 */
__attribute__((const))
__attribute__((overloadable))
bc_float2_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t minimum, bc_float_t maximum);

/**
\abstract Creates a 2D vertex suitable for drawing a portion of a cubic, making sure the portion has a minimum size.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param minimum the minimum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param maximum the maximum \c t to be generated.  In this way, vertexes can be generated for portions of a bezier curve without splitting.
 \param minimumDeltaT the minimum range to be drawn.  If \c maximum-minimum is less than this, the range will be adjusted.
 \throws Checks arguments.  rvalue is \c BCVertex2ErrorMake
 */
__attribute__((const))
__attribute__((overloadable))
bc_float2_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t minimum, bc_float_t maximum, bc_float_t minimumDeltaT);


/**
\abstract Creates a 2D vertex suitable for drawing an entire cubic.
\seealso See all overloads for this function.
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param transform A projection matrix to apply to the output vertex
 \throws Checks arguments.  rvalue is \c BCVertex3ErrorMake
 \returns 3d vertex
 */
__attribute__((const))
__attribute__((overloadable))
inline bc_float2_t BCCubicVertexMake(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic) {
    //rvalue is the same; passthrough
    return BCCubicVertexMake(cubic,vertexID, vertexesPerCubic, 0,1);
}

/**
 \abstract Creates a 3D vertex suitable for drawing a portion of a cubic in clip coordinates, specified in arclength coordinates (minimum and maximum position along the cubic).
 \seealso Other overloads for this function
 \param cubic Cubic to draw
 \param vertexID id of vertex we will draw, on the range \c 0..<vertexesPerCubic
 \param vertexesPerCubic highest vertex we will create for this cubic
 \param startPosition Start position to draw in linear coordinates.  If this is less than 0, it will be clamped to 0.
 \param endPosition End position to draw in linear coordinates.  This ought to be <= to the start coordinates.  If this exceeds the length of the cubic, it will be clamped.
 \param threshold The threshold to use for calculating the arclength parameterization.
 \param minimumDeltaT the minimum range to be drawn, in \c t coordinates.  If \c maximum-minimum is less than this, the range will be adjusted.
 \throws Checks arguments, \c rvalue.xyz is \c BCVertex3ErrorMake.  \c w is not defined.
 */
__attribute__((const))
bc_float2_t BCCubicVertexMakeClampedParameterization(BCCubic cubic, uint8_t vertexID, uint8_t vertexesPerCubic, bc_float_t startPosition, bc_float_t endPosition, bc_float_t threshold,bc_float_t minimumDelta);

#endif
