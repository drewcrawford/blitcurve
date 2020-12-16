// BCAlignedBox: Axis-aligned box
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedBox_h
#define BCAlignedBox_h
#include "BCTrap.h"
#include "BCTypes.h"
#include "BCCubic.h"
#include "BCStrategy.h"
#include "BCMacros.h"

#ifndef __METAL_VERSION__
#include <simd/simd.h>
#include <stdbool.h>
#endif


///@abstract An axis-aligned bounding box (AABB).
///@discussion An AlignedBox bounds, in some way, an underlying curve.  Depending on how it was constructed, the box may contain area significantly beyond the curve.
///Precomputation of boxes is useful for the early parts of mass intersection tests.
__attribute__((swift_name("AlignedRect")))
typedef struct {
    ///minimum coordinate in the box
    bc_float2_t min;
    ///maximum coordinate in the box
    bc_float2_t max;
    
} BCAlignedRect;

/**
 \abstract Calculates a bounding box for a cubic.
\param strategy The only value supported currently is \c fastest, which is O(1) and ~4 SIMD instructions
 \throws Checks the arguments with assert.  \c rvalue for the error case is a 0-sized rect.
 */
__attribute__((swift_name("AlignedRect.init(cubic:strategy:)")))
static inline BCAlignedRect BCAlignedRectCreateFromCubic(BCCubic c, BCStrategy strategy) {
    BCAlignedRect b;

    switch (strategy) {
        case BCStrategyFastest: {
            bc_float4_t x = bc_make_float4(c.a.x,c.b.x,c.c.x,c.d.x);
            bc_float4_t y = bc_make_float4(c.a.y,c.b.y,c.c.y,c.d.y);
            b.min = bc_make_float2(bc_reduce_min(x),bc_reduce_min(y));
            b.max = bc_make_float2(bc_reduce_max(x),bc_reduce_max(y));
            return b;
        }
            
        default: {
            b.min = 0;
            b.max = 0;
            __BC_ASSERT(false,b);
        }
    }
}


///\abstract Determines whether any point in a is within a given distance of any point in b.
///\note Does not test the center point
__attribute__((const))
__attribute__((swift_name("AlignedRect.isCornerWithinDistance(self:to:distance:)")))
bool BCAlignedRectsCornerWithinDistance(BCAlignedRect a, BCAlignedRect b,bc_float_t distance);

///\abstract Gets the center of the box
__attribute__((const))
__attribute__((swift_name("getter:AlignedRect.center(self:)")))
inline bc_float2_t BCAlignedRectCenterPoint(BCAlignedRect a) {
    bc_float2_t sum = a.max - a.min;
    return sum / 2 + a.min;
}

///\abstract Determines whether the given point is on or inside the rect.
__attribute__((const))
__attribute__((swift_name("AlignedRect.isPointOnOrInside(self:_:)")))
inline bool BCAlignedRectIsPointOnOrInside(BCAlignedRect a, bc_float2_t point) {
    return (a.min.x <= point.x && a.max.x >= point.x && a.min.y <= point.y && a.max.y >= point.y);
}
#endif
