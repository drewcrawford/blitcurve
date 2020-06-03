// BCCubic.c: Cubic implementation
// ©2020 DrewCrawfordApps LLC

#include "BCCubic.h"

extern inline BCLine BCCubicFinalTangent(BCCubic c);
extern inline BCLine BCCubicInitialTangent(BCCubic c);
extern inline bc_float2_t evaluate(BCCubic c,bc_float_t t);
extern inline BCLine BCCubicAsLine(BCCubic c);
extern inline void BCCubicNormalize(BCCubic *c);
