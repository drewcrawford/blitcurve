// BCAlignedBox: Axis-aligned box
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedBox_h
#define BCAlignedBox_h
#import "BCTypes.h"
#import "BCCubic.h"
#import "BCStrategy.h"
#import "BCMacros.h"
#import <simd/simd.h>
#import <stdbool.h>


///An axis-aligned bounding box (AABB).
///An AlignedBox bounds, in some way, an underlying curve.  Depending on how it was constructed, the box may contain area significantly beyond the curve.
///Precomputation of boxes is useful for the early parts of mass intersection tests.
__attribute__((swift_name("AlignedBox")))
typedef struct {
    ///minimum coordinate in the box
    bc_float2_t min;
    ///maximum coordinate in the box
    bc_float2_t max;
    
} BCAlignedBox;

///Calculates a bounding box for a cubic.
///- parameter strategy: The only value supported currently is `fastest`, which is O(1) and ~4 SIMD instructions
__attribute__((swift_name("AlignedBox.init(cubic:strategy:)")))
static inline BCAlignedBox BCAlignedBoxCreateFromCubic(BCCubic c, BCStrategy strategy) {
    BCAlignedBox b;

    switch (strategy) {
        case BCStrategyFastest: {
            bc_float4_t x = simd_make_float4(c.a.x,c.b.x,c.c.x,c.d.x);
            bc_float4_t y = simd_make_float4(c.a.y,c.b.y,c.c.y,c.d.y);
            b.min = simd_make_float2(simd_reduce_min(x),simd_reduce_min(y));
            b.max = simd_make_float2(simd_reduce_max(x),simd_reduce_max(y));
            return b;
        }
            
        default:
        ASSERT_UB(false);
    }
}

#endif
