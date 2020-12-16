//BCAlignedCubic.h: Aligned cubic definition
// ©2020 DrewCrawfordApps LLC

#ifndef BCAlignedCubic_h
#define BCAlignedCubic_h
#include "BCTypes.h"
#include "BCCubic.h"
#include "BCMacros.h"
/**Cubics can be "aligned" by applying translations and rotations that put them into a canonical form.  While we lose relative position about the cubic,
we retain information intrinsic to the cubic.  In addition, certain operations are simplified.
 @discussion There are various algorithms that can be implemented more quickly in the situation that the cubic is "aligned".  Aligning a cubic effectively creates a new coordinate space that cannot be naively compared to the global coordinate space.  However, the cubic still maintains its internal properties, like length and shape, and operations like that return the same result with less work.
 
 This creates several opportunities for client optimizations.  Many cubics can map to a single aligned representation, which can be shared between them.  In addition, the cubic can be aligned once and the alignment can be re-used for many operations.  Exploiting this is however up to the client application.
 
 In other cases, the alignment + algorithm may be the fastest implementation for a function, even if the alignment is not reused.
 
Aligned cubics have the following properties:
 
1. \c a is at 0,0 (cubic is translated).  For this reason, there is no \c a coordinate.
 
2. \c b.y is 0 (cubic is rotated).  For this reason, \c b is represented only by  \c b_x.
 
 In this situation, the evaluation function becomes
 
 @code
 b(x) = 3 * c * (1-t)^2 * t + 3 * d * (1-t) * t^2 + b * t^3
 b(y) = 3 * c * (1-t)^2 * t + 3 * d * (1-t) * t^2
 @endcode
 */
__attribute__((swift_name("AlignedCubic")))
typedef struct {
    //note: this is listed in an order optimal for struct packing
    bc_float2_t c;
    bc_float2_t d;
    bc_float_t b_x;
} BCAlignedCubic;

/**  Aligns a cubic.
 @see See the documentation for BCAlignedCubic for details on why you would do this.
 @param c The cubic to align.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.init(cubic:)")))
BCAlignedCubic BCAlignedCubicMake(BCCubic c);

/**
 This calculates the kappa function (curvature).
 @discussion This returns the curvature of the curve at a specific bezier parameter.  The turning radius is \c 1/abs(kappa).
 
 @warning This function is UB if the curve has 0-length or the cubic not technically normalized (see \c BCCubicIsTechnicallyNormalized)`  In addition, "good behavior" requires a higher-than-normal normalization distance, see \c BCAlignedCubicIsNormalizedForCurvature for details.

 @param t The bezier parameter on interval [0,1] at which to measure the curvature.
 @throws Checks arguments, rvalue is \c BC_FLOAT_LARGE.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.kappa(self:t:)")))
bc_float_t BCAlignedCubicKappa(BCAlignedCubic c, bc_float_t t);

#ifndef NDEBUG
__attribute__((const))
__attribute__((swift_name("AlignedCubic.__kappaPrime(self:t:)")))
/**
 Private function.
 @discussion uses \c BUGIF to check arguments, rvalue is \c BC_FLOAT_LARGE.
 */
bc_float_t __BCAlignedCubicKappaPrime(BCAlignedCubic c, bc_float_t t);
#endif

/**This returns the bezier parameter \c (t) at which the maximum kappa can be found.
 @param c the cubic
 @param accuracy The maximum error allowed on \c t
 @performance This implements a binary search.
 @warning This function is UB if the curve has 0-length or the cubic not technically normalized (see \c BCCubicIsTechnicallyNormalized)`  In addition, "good behavior" requires a higher-than-normal normalization distance, see \c BCAlignedCubicIsNormalizedForCurvature for details.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.maxKappaParameter(self:accuracy:)")))
bc_float_t BCAlignedCubicMaxKappaParameter(BCAlignedCubic c, bc_float_t accuracy);

/** This returns the curvature radius at a given bezier parameter.
 @see This is the inverse of BCAlignedCubicKappa.
 @param t The bezier parameter on interval [0,1] at which to measure the curvature.
 @warning This function is UB if the curve has 0-length or the cubic not technically normalized (see \c BCCubicIsTechnicallyNormalized)`  In addition, "good behavior" requires a higher-than-normal normalization
distance, see \c BCAlignedCubicIsNormalizedForCurvature for details.
 @throws Checks arguments, rvalue is \c BC_FLOAT_LARGE.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.curveRadius(self:t:)")))
inline bc_float_t BCAlignedCubicCurveRadius(BCAlignedCubic c, bc_float_t t)
__attribute__((diagnose_if(!(t>=0&&t<=1), "bezier parameter out of range","error")))
{
    __BC_RANGEASSERT(t>=0 && t <=1,BC_FLOAT_LARGE);
    const float kappa = BCAlignedCubicKappa(c, t);
    __BC_BUGASSERT(kappa!=0, BC_FLOAT_LARGE);
    return 1.0 / kappa;
}

/**Determines if the given cubic is normalized with a method appropriate for curvature calculations.
 \discussion See the documentation for \c BCCubicNormalize and \c BCNormalizationDistanceForCubicCurvatureError
 \seealso see \c BCCubicIsNormalizedForCurvature, a variant for the \c BCCubic case.
 @param cubic a cubic to check
 @param straightAngle Positive angle that is "nearly straight", used to detect the undesired curvature.  Plausible values for this parameter include \c 2*PI/360f
 @param curvatureError  maximum error (difference in unsigned curvature for a "nearly straight" cubic) we want.  I think this function is not well-behaved if you pass a value below \c 1*10^-14 or so, so if you intended to do that, just use \c BCLineLength(BCCubicAsLine(cubic))/2 rather than calling this function.
 @returns whether the cubic is normalized in a way appropriate for curvature calculations.
 */
__attribute__((const))
__attribute__((swift_name("AlignedCubic.isNormalizedForCurvature(self:straightAngle:curvatureError:)")))
bool BCAlignedCubicIsNormalizedForCurvature(BCAlignedCubic cubic, bc_float_t straightAngle, bc_float_t curvatureError)
__attribute__((diagnose_if(!(cubic.b_x>0), "Invalid distance","error")))
__attribute__((diagnose_if(!(straightAngle>0), "Invalid straightAngle","error")))
__attribute__((diagnose_if(!(curvatureError>0), "Invalid curvatureError","error")));


#endif



