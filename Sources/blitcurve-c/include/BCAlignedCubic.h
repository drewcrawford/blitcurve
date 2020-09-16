//BCAlignedCubic.h: Aligned cubic definition
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedCubic_h
#define BCAlignedCubic_h
#include "BCTypes.h"
#include "BCCubic.h"
#include "BCMacros.h"
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

/**
 This calculates the kappa function (curvature).
 @discussion This returns the curvature of the curve at a specific bezier parameter.  The turning radius is \c 1/abs(kappa).
 @param t The bezier parameter on interval [0,1] at which to measure the curvature.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.kappa(self:t:)")))
bc_float_t BCAlignedCubicKappa(BCAlignedCubic c, bc_float_t t)
__attribute__((diagnose_if(!(t>=0&&t<=1), "bezier parameter out of range","error")));

#ifndef NDEBUG
__attribute__((const))
__attribute__((swift_name("AlignedCubic.__kappaPrime(self:t:)")))
bc_float_t __BCAlignedCubicKappaPrime(BCAlignedCubic c, bc_float_t t);
#endif

__attribute__((const))
__attribute__((swift_name("AlignedCubic.maxKappaParameter(self:accuracy:)")))
bc_float_t BCAlignedCubicMaxKappaParameter(BCAlignedCubic c, bc_float_t accuracy);

__attribute__((const))
__attribute__((swift_name("AlignedCubic.curveRadius(self:t:)")))
inline bc_float_t BCAlignedCubicCurveRadius(BCAlignedCubic c, bc_float_t t) {
    return 1.0 / BCAlignedCubicKappa(c, t);
}

#endif



