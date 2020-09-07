// BCBezierParameter.c: Bezier parameter implementations
// ©2020 DrewCrawfordApps LLC
#import "BCBezierParameter.h"
__attribute__((overloadable))
extern inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance);

__attribute__((overloadable))
extern inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance, float lowerT, float upperT);

__attribute__((overloadable))
extern inline bc_float_t BCLUTParameterForIndex(uint8_t lutIndex, uint8_t lutCapacity, float lowerT, float upperT);
__attribute__((overloadable))
extern inline bc_float_t BCLUTParameterForIndex(uint8_t lutIndex, uint8_t lutCapacity);

__attribute__((overloadable))
extern inline uint8_t BCLUTIndexForParameter(float t, uint8_t lutCapacity, BCLUTRoundingMode roundingMode, float lowerT, float upperT);

__attribute__((overloadable))
extern inline uint8_t BCLUTIndexForParameter(float t, uint8_t lutCapacity, BCLUTRoundingMode roundingMode);
