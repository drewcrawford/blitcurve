// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC
#include "BCTypes.h"
#ifndef Cubic_h
#define Cubic_h

#include <stdbool.h>
///BCCubic is a cubic bezier curve defined on 4 points.
///Unlike some other notation, we use `a` and `b` consistently for start and end points, reserving other values for control points.
__attribute__((swift_name("Cubic")))
typedef struct {
    ///start of curve
    bc_float2_t a;
    ///end of curve
    bc_float2_t b;
    ///control point 1
    bc_float2_t c;
    ///control point 2
    bc_float2_t d;
} BCCubic;

__attribute__((swift_name("Cubic.init(connecting:to:)")))
BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b);

__attribute__((swift_name("Cubic.init(connecting:initialTangent:finalTangent:)")))
BCCubic BCCubicMakeConnectingTangents(BCCubic connecting, BCCubic initialTangent, BCCubic finalTangent);

__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic a);

__attribute__((swift_name("Cubic.hasIntersection(self:with:)")))
bool BCCubicHasIntersection(BCCubic a, BCCubic b);

__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
BCLine LCCubicFinalTangent(BCCubic a);

#endif

