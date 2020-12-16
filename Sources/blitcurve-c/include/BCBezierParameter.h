//BCBezierParameter.h: Bezier parameter functions
// ©2020 DrewCrawfordApps LLC
#ifndef BCBezierParameter_h
#define BCBezierParameter_h
#include "BCTypes.h"
#include "BCMacros.h"
#include "BCMetalC.h"
#include "BCTrap.h"

/**\abstract Converts between a vertex ID and a bezier parameter.
\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.  This must be \c <vertexesPerInstance.
 \param vertexesPerInstance The number of vertexes to draw for each instance.  This must be \c >1
 \throws Checks arguments with assert, if invalid will return (-1-BCError).
@see BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT2(vertexID < vertexesPerInstance, (-1-BCErrorArgRelationship));
    __BC_ASSERT2(vertexesPerInstance > 1,(-1-BCErrorArg1));
    return ((float)vertexID) / (vertexesPerInstance - 1);
}

/**
\abstract Converts between a vertex ID and a bezier parameter, with a custom min and max \c t.
\discussion This is primarily useful for the case that you want to evaluate points in a region of the curve, but without splitting the curve.  Splitting the curve is generally more efficient if the split can be used across multiple points, but that would require additional memory.
\param vertexID The vertex id, e.g. a shader parameter.
\param vertexesPerInstance The number of vertexes to draw for each instance.
\param lowerT The return value will be \c >= this number
\param upperT The return value will be \c <= this number.
 \throws Checks arguments with assert, if invalid will return (-1-BCError).
 @see \c BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:lowerT:upperT:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance, float lowerT, float upperT)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT2(vertexID < vertexesPerInstance,(1-BCErrorArgRelationship));
    __BC_ASSERT2(lowerT < upperT,(1-BCErrorRangeIsZero));
    __BC_ASSERT2(vertexesPerInstance > 1,(1-BCErrorArg1));
    float range = upperT - lowerT;
    return ((float)vertexID) / (vertexesPerInstance - 1) * range + lowerT;
}
#endif
