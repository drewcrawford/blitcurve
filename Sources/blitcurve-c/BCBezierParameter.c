// BCBezierParameter.c: Bezier parameter implementations
// ©2020 DrewCrawfordApps LLC
#import "BCBezierParameter.h"
__attribute__((overloadable))
extern inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, char vertexesPerInstance);

__attribute__((overloadable))
extern inline bc_float_t BCVertexToBezierParameterWithBounds(uint16_t vertexID, char vertexesPerInstance, float lowerT, float upperT);

extern inline bc_float_t BCVertexToBezierParameter(uint32_t vertexID, char vertexesPerInstance);
