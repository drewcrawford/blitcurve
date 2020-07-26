// BCAlignedBox: Axis-aligned box
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedBox_h
#define BCAlignedBox_h
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

///\abstract Calculates a bounding box for a cubic.
///\param strategy The only value supported currently is \c fastest, which is O(1) and ~4 SIMD instructions
__attribute__((swift_name("AlignedRect.init(cubic:strategy:)")))
static inline BCAlignedRect BCAlignedRectCreateFromCubic(BCCubic c, BCStrategy strategy) {
    BCAlignedRect b;

    switch (strategy) {
        case BCStrategyFastest: {
            bc_float4_t x = simd_make_float4(c.a.x,c.b.x,c.c.x,c.d.x);
            bc_float4_t y = simd_make_float4(c.a.y,c.b.y,c.c.y,c.d.y);
            b.min = simd_make_float2(simd_reduce_min(x),simd_reduce_min(y));
            b.max = simd_make_float2(simd_reduce_max(x),simd_reduce_max(y));
            return b;
        }
            
        default:
        __BC_ASSERT(false);
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
static inline bc_float2_t BCAlignedRectCenterPoint(BCAlignedRect a) {
    simd_float2 sum = a.max - a.min;
    return sum / 2 + a.min;
}

#endif
