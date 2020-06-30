//BCBezierParameter.h: Bezier parameter functions
// ©2020 DrewCrawfordApps LLC
#import "BCTypes.h"
#import "BCMacros.h"

///\abstract Converts between a vertex ID and a bezier parameter.
///\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.
///\param vertexesPerInstance The number of vertexes to draw for each instance.
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, char vertexesPerInstance) {
    float vertexID_f = vertexID % vertexesPerInstance;
    return ((float)vertexID_f) / (vertexesPerInstance - 1);
}

///\abstract Converts between a vertex ID and a bezier parameter.
///\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.
///\param vertexesPerInstance The number of vertexes to draw for each instance.
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
__attribute__((const))
inline bc_float_t BCVertexToBezierParameter(uint32_t vertexID, char vertexesPerInstance) {
    float vertexID_f = vertexID % vertexesPerInstance;
    return ((float)vertexID_f) / (vertexesPerInstance - 1);
}

///\abstract Converts between a vertex ID and a bezier parameter, with a custom min and max \c t.
///\discussion This is primarily useful for the case that you want to evaluate points in a region of the curve, but without splitting the curve.  Splitting the curve is generally more efficient if the split can be used across multiple points, but that would require additional memory.
///\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.
///\param vertexesPerInstance The number of vertexes to draw for each instance.
///\param lowerT The return value will be \c >= this number
///\param upperT The return value will be \c <= this number.
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:lowerT:upperT:)")))
inline bc_float_t BCVertexToBezierParameterWithBounds(uint16_t vertexID, char vertexesPerInstance, float lowerT, float upperT) {
    __BC_ASSERT(lowerT < upperT);
    float vertexID_f = vertexID % vertexesPerInstance;
    float range = upperT - lowerT;
    return ((float)vertexID_f) / (vertexesPerInstance - 1) * range + lowerT;
}


