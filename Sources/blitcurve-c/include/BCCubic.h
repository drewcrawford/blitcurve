// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC
#include "BCTypes.h"
#ifndef Cubic_h
#define Cubic_h

#include <stdbool.h>
__attribute__((swift_name("Cubic")))
typedef struct {
    bc_float2_t a;
    bc_float2_t b;
    bc_float2_t c;
    bc_float2_t d;
} BCCubic;

//https://github.com/twostraws/swift-1/blob/master/docs/CToSwiftNameTranslation.md

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

