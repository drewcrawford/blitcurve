//BCBezierParameter.h: Bezier parameter functions
// ©2020 DrewCrawfordApps LLC
#ifndef BCBezierParameter_h
#define BCBezierParameter_h
#include "BCTypes.h"
#include "BCMacros.h"
#include "BCMetalC.h"

/**\abstract Converts between a vertex ID and a bezier parameter.
\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.  This must be \c <vertexesPerInstance.
 \param vertexesPerInstance The number of vertexes to draw for each instance.
@see BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT(vertexID < vertexesPerInstance);
    __BC_ASSERT(vertexesPerInstance > 1);
    return ((float)vertexID) / (vertexesPerInstance - 1);
}

/**
\abstract Converts between a vertex ID and a bezier parameter, with a custom min and max \c t.
\discussion This is primarily useful for the case that you want to evaluate points in a region of the curve, but without splitting the curve.  Splitting the curve is generally more efficient if the split can be used across multiple points, but that would require additional memory.
\param vertexID The vertex id, e.g. a shader parameter.
\param vertexesPerInstance The number of vertexes to draw for each instance.
\param lowerT The return value will be \c >= this number
\param upperT The return value will be \c <= this number.
 @see \c BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:lowerT:upperT:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance, float lowerT, float upperT)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(lowerT < upperT), "bezier upper/lower out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT(vertexID < vertexesPerInstance);
    __BC_ASSERT(lowerT < upperT);
    __BC_ASSERT(vertexesPerInstance > 1);
    float range = upperT - lowerT;
    return ((float)vertexID) / (vertexesPerInstance - 1) * range + lowerT;
}
#endif
