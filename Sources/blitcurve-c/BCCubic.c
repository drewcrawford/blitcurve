// BCCubic.c: Cubic implementation
// ©2020 DrewCrawfordApps LLC

#include "BCCubic.h"

extern inline BCLine BCCubicFinalTangentLine(BCCubic c);
extern inline BCLine BCCubicInitialTangentLine(BCCubic c);
extern inline bc_float2_t evaluate(BCCubic c,bc_float_t t);
extern inline BCLine BCCubicAsLine(BCCubic c);
extern inline void BCCubicNormalize(BCCubic *c);
extern inline bc_float_t BCCubicInitialTangent(BCCubic c);
extern inline bc_float_t BCCubicFinalTangent(BCCubic c);
extern inline BCCubic BCCubicMakeConnectingTangents(BCLine connecting, bc_float_t initialTangent, bc_float_t finalTangent);
extern inline bc_float_t BCCubicLength(BCCubic c);
