//BCAlignedCubic.h: Aligned cubic definition
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedCubic_h
#define BCAlignedCubic_h
#include "BCTypes.h"
#include "BCCubic.h"
/**Cubics can be "aligned" by applying translations and rotations that put them into a canonical form.  While we lose relative position about the cubic,
we retain information intrinsic to the cubic.  In addition, certain operations are simplified.
 
 \discussion We align the cubic by
    1. Setting it's 'a' coordinate to 0.  This eliminates the need to track a.
    2. Rotating until the 'b' coordinate to be (bx,0).  This eliminates the need to track b.y.
 */
__attribute__((swift_name("AlignedCubic")))
typedef struct {
    //note: this is listed in an order optimal for struct packing
    bc_float2_t c;
    bc_float2_t d;
    bc_float_t b_x;
} BCAlignedCubic;

__attribute__((const))
__attribute__((swift_name("AlignedCubic.init(cubic:)")))
BCAlignedCubic BCAlignedCubicMake(BCCubic c);

__attribute__((const))
__attribute__((swift_name("AlignedCubic.kappa(self:t:)")))
bc_float_t BCAlignedCubicKappa(BCAlignedCubic c, bc_float_t t);
#endif

