//BCBezierParameter.h: Bezier parameter functions
// ©2020 DrewCrawfordApps LLC
#import "BCTypes.h"

///\abstract Converts between a vertex ID and a bezier parameter.
///\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.
///\param vertexesPerInstance The number of vertexes to draw for each instance.
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, char vertexesPerInstance) {
    return ((float)vertexID) / (vertexesPerInstance - 1);
}

///\abstract Converts between a vertex ID and a bezier parameter.
///\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.
///\param vertexesPerInstance The number of vertexes to draw for each instance.
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
__attribute__((const))
inline bc_float_t BCVertexToBezierParameter(uint32_t vertexID, char vertexesPerInstance) {
    return ((float)vertexID) / (vertexesPerInstance - 1);
}
