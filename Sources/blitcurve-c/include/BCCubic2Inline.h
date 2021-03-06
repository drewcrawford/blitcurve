//BCCubic2Inline.h: Inline functions for BCCubic
// ©2020 DrewCrawfordApps LLC
#ifndef BCCubic2Inline_h
#define BCCubic2Inline_h
#include "BCCubic2.h"
#include "BCCubic.h"
__attribute__((const))
__attribute__((swift_name("getter:BCCubic2.left(self:)")))
///@abstract Gets the left cubic out of \c BCCubic2
inline BCCubic BCCubic2SeparateLeft(BCCubic2 c) {
    BCCubic out;
    out.a = c.a.xy;
    out.b = c.b.xy;
    out.c = c.c.xy;
    out.d = c.d.xy;
    return out;
}

__attribute__((const))
__attribute__((swift_name("getter:BCCubic2.right(self:)")))
///@abstract Gets the right cubic out of \c BCCubic2
inline BCCubic BCCubic2SeparateRight(BCCubic2 c) {
    BCCubic out;
    out.a = c.a.zw;
    out.b = c.b.zw;
    out.c = c.c.zw;
    out.d = c.d.zw;
    return out;
}
#endif
