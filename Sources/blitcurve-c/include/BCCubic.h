// BCCubic.h: Cubic type
// ©2020 DrewCrawfordApps LLC

#ifndef Cubic_h
#define Cubic_h
#include "BCMetalC.h"

#include "BCTypes.h"
#include "BCLine.h"
#include "BCMath.h"
#include "BCLine2.h"
#include "BCCubic2.h"
#include "BCTrap.h"

#ifndef __METAL_VERSION__
#include <stdbool.h>
#include <float.h>
#endif

#include <simd/simd.h>
///\abstract BCCubic is a cubic bezier curve defined on 4 points.
///\discussion Unlike some other notation, we use \c a and \c b consistently for start and end points, reserving other values for control points.
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

///Creates an "error cubic" to describe a \c BCError as a \c BCCubic return value.
static inline BCCubic BCErrorCubicMake(BCError error) {
    BCCubic r;
    r.a = bc_make_float2(BC_FLOAT_LARGE,BC_FLOAT_LARGE);
    r.b = bc_make_float2(BC_FLOAT_LARGE,BC_FLOAT_LARGE);
    r.c = bc_make_float2(BC_FLOAT_LARGE,BC_FLOAT_LARGE);
    r.d = bc_make_float2(error,error);
    return r;
}

///\abstract Returns the line from a to c.
///\warning In the case that \code a =~= c \endcode, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentLine(self:)")))
inline BCLine BCCubicInitialTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.c;
    return l;
}

///\abstract Returns the line from d to b.
///\warning In the case that \code d=~= b \endcode, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentLine(self:)")))
inline BCLine BCCubicFinalTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.d;
    l.b = c.b;
    return l;
}

/**\abstract Returns the angle of the \c intitalTangent.
\throws This requires the cubic to be partially normalized between \c a/c, see \c BCCubicNormalize for details.  Checks parameters, rvalue is \c BC_FLOAT_LARGE.
 
*/
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentAngle(self:)")))
static inline bc_float_t BCCubicInitialTangentAngle(BCCubic c) {
    //BCLineTangent and we share same rvalue, so we just pass through
    return BCLineTangent(BCCubicInitialTangentLine(c));
}

///\abstract Returns the  magnitude of the \c intitalTangent.
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentMagnitude(self:)")))
static inline bc_float_t BCCubicInitialTangentMagnitude(BCCubic c) {
    return BCLineLength(BCCubicInitialTangentLine(c));
}

/**\abstract Returns the angle of the \c finalTangent.
\throws This requires the cubic to be partially normalized between \c a/c, see \c BCCubicNormalize for details.  Checks parameters, rvalue is \c BC_FLOAT_LARGE.
 */
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentAngle(self:)")))
static inline bc_float_t BCCubicFinalTangentAngle(BCCubic c) {
    //BCLineTangent and we share same rvalue, so we just pass through
    return BCLineTangent(BCCubicFinalTangentLine(c));
}



///\abstract Returns the  magnitude of the \c finalTangent.
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentMagnitude(self:)")))
static inline bc_float_t BCCubicFinalTangentMagnitude(BCCubic c) {
    return BCLineLength(BCCubicFinalTangentLine(c));
}

///\abstract Evaluate the cubic for the given bezier parameter
///\return A point on the cubic at the bezier equation for \c t
__attribute__((const))
__attribute__((swift_name("Cubic.evaluate(self:t:)")))
inline bc_float2_t BCCubicEvaluate(BCCubic c,bc_float_t t) {
    return c.a * __bc_cube(1-t) + c.c * 3 * __bc_square(1-t) * t + c.d * 3 * (1 - t) * __bc_square(t) + c.b * __bc_cube(t);
}
__attribute__((const))
__attribute__((swift_name("Cubic.evaluatePrime(self:t:)")))
///\abstract Evaluate the derivative of a BCCubic
///\return A point on the derivative of the given cubic at the given bezier parameter.
inline bc_float2_t BCCubicEvaluatePrime(BCCubic c, bc_float_t t) {
    //D[a * (1-t)^3 + c * 3 * (1-t)^2 * t + d * 3 * (1 - t) * t^2 + b * t^3,t]
    //-3 a (1-t)^2+3 c (1-t)^2-6 c (1-t) t+6 d (1-t) t+3 b t^2-3 d t^2
    //-3 * c.a * pow(1-t,2) + 3 * c.c * pow(1-t,2) - 6 * c.c * (1-t) * t + 6 * c.d * (1 - t) * t + 3 * c.b * pow(t,2) - 3 * c.d * pow(t,2);
    const float one_minus_t = 1 - t;
    const float three_by_one_minus_t_squared = 3 * __bc_square(one_minus_t);
    const float three_by_t_squared = 3 * __bc_square(t);
    const float six_by_1_minus_t_t = 6 * one_minus_t * t;
    return -three_by_one_minus_t_squared * c.a + three_by_one_minus_t_squared * c.c - six_by_1_minus_t_t * c.c + six_by_1_minus_t_t * c.d + three_by_t_squared * c.b - three_by_t_squared * c.d;
}

__attribute__((const))
__attribute__((swift_name("Cubic.isTechnicallyNormalized(self:)")))
///\abstract Checks if the cubic is technically normalized
///\discussion See the documentation for BCCubicNormalize for a discussion of this operation.
///
///\note In general, it's preferable to call an underlying function to perform a \a partial normalization check.  However, in some cases
///this does not make sense.
///In that situation, you can \c assert_expression(BCCubicIsNormalized(cubic)) instead.
static inline bool BCCubicIsTechnicallyNormalized(BCCubic c) {
    return bc_distance(c.c, c.a) != 0 && bc_distance(c.b, c.d)  != 0;
}

__attribute__((const))
__attribute__((swift_name("NormalizationDistanceForCubicCurvature(euclidianDistance:straightAngle:curvatureError:)")))
/**
 \abstract Finds the minimum normalization distance required to get "nice" curvature behavior for \c BCCubic or \c BCAlignedCubic.
 \discussion Many bezier functions are undefined in the case the control points get near the endpoints, which is why we have \c BCCubicNormalize and related.  This UB is the case for the curvature (\c kappa and related) functions as well.  However, the curvature is unique because even in cases where there it seems like there's a reasonable distance between points (and so the resulting curvature is "well-defined"), the behavior of curvature can be "strange".  This usually presents with a very high curvature at the endpoints, which can be misleading if you are evaluating cubics near endpoints or finding maximum curvature along a cubic, or doing something that relies on that.
 
 The solution to this is to increase the distance between control points and endpoints, beyond what might be a reasonable distance for "well-defined" behavior.  This function computes the distance required.  The upper-bound of this function is \c euclidianDistance/2, so that is an appropriate value for cases where this function is difficult.
 
 A discussion of this problem with various plots of the "strange" behavior appears at https://sealedabstract.com/posts/bezier-curvature/
 
 @param euclidianDistance positive distance between \c a and \c b.
 @param straightAngle Positive angle that is "nearly straight", used to detect the undesired curvature.  Plausible values for this parameter include \c 2*PI/360f
 @param curvatureError  maximum error (difference in unsigned curvature for a "nearly straight" cubic) we want.  I think this function is not well-behaved if you pass a value below \c 1*10^-14 or so, so if you intended to do that, just use \c euclidianDistance/2 rather than calling this function.
 
 @returns a normalization distance, to find the required distance between endpoints to get "nice" curvature.
 @throws (-1-BCError) to report errors in arguments.
 */
bc_float_t BCNormalizationDistanceForCubicCurvatureError(bc_float_t euclidianDistance, bc_float_t straightAngle, bc_float_t curvatureError);







__attribute__((const))
__attribute__((swift_name("getter:Cubic.asLine(self:)")))
///\abstract Get a line from the \c BCCubic
inline BCLine BCCubicAsLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.b;
    return l;
}



/**\abstract Returns whether the cubic is nearly a line (within some accuracy).
 \returns 0 if not a line, 1 if a line.  See also throws documentation.
 \throws Asserts to check arguments, rvalue is \c (-1-BCError)
 */
__attribute__((const))
__attribute__((swift_name("Cubic.isNearlyLinear(self:accuracy:)")))
static inline char BCCubicIsNearlyLinear(BCCubic self, bc_float_t accuracy) {
    __BC_ASSERT(accuracy > 0,(-1-BCErrorArg1));
    
    const bc_float_t lineTangent = BCLineTangent(BCCubicAsLine(self));
    __BC_PRECONDITION_CONVERT(lineTangent==BC_FLOAT_LARGE,(-1-BCErrorArg0));
    return (bc_abs(lineTangent - BCCubicInitialTangentAngle(self)) < accuracy && bc_abs(lineTangent - BCCubicFinalTangentAngle(self)) < accuracy);
}

__attribute__((const))
__attribute__((swift_name("Cubic.tangentAt(self:t:)")))
/**
 \abstract Evaluates the tangent at the given point
\throws Asserts the cubic is normalized.  rvalue is \c BC_FLOAT_LARGE.
 */
static inline bc_float_t BCCubicTangent(BCCubic c, bc_float_t t) {
    __BC_PRECONDITION(BCCubicIsTechnicallyNormalized(c), BC_FLOAT_LARGE);
    bc_float2_t prime = BCCubicEvaluatePrime(c, t);
    return bc_atan2(prime.y, prime.x);
}

__attribute__((swift_name("Cubic.normalize(self:approximateDistance:)")))
/**\abstract Normalize the cubic
\discussion When a control point is exactly (or in some contexts, merely 'near') an endpoint, it creates various problems.  For example, the definition of an initial tangent is the angle between \c a and \c c, but if \c a=c this angle is undefined.  We call such difficult curves  \a non-normalized.

We say a curve is \a partially normalized to refer to normalization between some subset of endpoints and control points that are required to mathematically define a particular operation.  For example, as discussed \c intialTangent requires a \a partial \a normalization for \c a and \c c.

We say a curve is \a technically normalized if there is some nonzero distance between all related endpoints and control points.  This generally provides a mathematical definition for operations on the curve.

In practice, applications may want a curve to be \a generously normalized.  For example, a small distance between \c a and \c c may technically define \c initialTangent at \c t=0, but the value is not necessarily close to the tangent at \c t=0.01, so an application scanning the bezier parameters at "reasonable" intervals may see weird behavior.  For this reason an application may want to normalize more generously to provide a larger distance between endpoints and control points.
One particularly important case is the situation of calculating curvature.  See \c BCNormalizationDistanceForCubicCurvatureError for a function to calculate a normalization distance for this case.
\param c The cubic to normalize.  We will normalize it in-place.
\param approximateDistance the desired distance between endpoints and control points.  Pass a positive nonzero value.  Note the following cases:
1.  If the control/endpoints are already at least this distance, we will not alter them
2.  If the control/endpoints are less than this distance, we will adjust the control points to a value approximately this distance, but it will not be exact, and the value we choose may be higher or lower.  For this reason, avoid values near the working precision, such as \c FLT_MIN, and avoid treating this as a minimum distance.
\return A normalized curve, with at least the distance specified between related endpoints and control points.
 \throws Will set the cubic to \c BCCubicError.
 */
void BCCubicNormalize(BCCubic __BC_DEVICE *c, bc_float_t approximateDistance);


__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:tangents:distances:)")))
/**Create a cubic that connects a line with initial and final tangents
\param connecting The line to connect with
\param tangents in (initial,final) orientation
\param distances normalization distances in (initial,final) orientation.
\throws Asserts to check arguments.  rvalue is \c BCErrorCubicMake(BCError).
 
 */
static inline BCCubic BCCubicMakeConnectingTangents(BCLine connecting, bc_float2_t tangents, bc_float2_t distances) {
    const BCCubic errorCubic = {BC_FLOAT_LARGE,BC_FLOAT_LARGE,BC_FLOAT_LARGE,BCErrorArg0};
    __BC_PRECONDITION(BCLineLength(connecting) > 0, BCErrorCubicMake(BCErrorArg0));
    BCCubic c;
    c.a = connecting.a;
    c.b = connecting.b;
    
    /*
                
    pick c/d such that it's collinear to the tangent
    keep in mind the following 5 cases
      ▲               ▲
       ╲               ╲
       ft    .          ft   .
         ╲  ( )          ╲  ( )
          ╲  '            ╲  '
          ▲               ▲
          │               │
        line             line
          │               │
          │   .       .   │
          ▲  ( )     ( )  ▲
         ╱    '       '    ╲
       it               it  ╲
       ╱                     ╲
      ╱                       ╲
                               ╲
                                                    
                                              ▲
      .     ▲               ▲                 │
     ( )   ╱               ╱                  │
      '   ╱         . ft  ╱                 ft│
     ft  ╱         ( )   ╱                    │
        ╱           '   ╱                     │
        ▲               ▲                     ▲.
        │               │                     ( )
        │              line                  l.'e
      line              │                    ( )
        │   .       .   │                     '
        ▲  ( )     ( )  ▲                     ▲
       ╱    '       '    ╲                    │
      ╱               it  ╲                 it│
     it                    ╲                  │
    ╱                       ╲                 │
                             ╲                │
    
    The core idea here is we try to figure out the IT/FT are to the right or left of the line
    and place the control point on the opposite side.
     Note that to get ft like it is in this diagram, we need to reverse the arg
     
        */
    //find the difference between angles in (i,f) format
    tangents.y -= BC_M_PI_F;
    BCLine2 i_f = BCLine2MakeWithPointAndAngle(bc_make_float4(connecting.a,connecting.b),tangents,distances);
    c.c = i_f.b.xy;
    c.d = i_f.b.zw;
    return c;
}

/**Creates a cubic connecting two lines, with an initialTangent [finalTangent of the a] and finalTangent [reversed initialTangent of B]
\discussion This will choose the initial and final tangents with mangitudes \c r/2.  For more details on this choice, see https://sealedabstract.com/posts/bezier-curvature/
\throws Requires lines to have non-zero distance, rvalue is \c BCErrorCubic.
 */
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:)")))
static inline BCCubic BCCubicMakeConnectingLines(BCLine a, BCLine b) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b.a;
    const bc_float_t distance = bc_distance(a.b,b.a)/2;
    
    const bc_float_t tangentA = BCLineTangent(a);
    __BC_PRECONDITION_CONVERT(tangentA==BC_FLOAT_LARGE,BCErrorCubicMake(BCErrorArg0));
    const bc_float_t tangentB = BCLineTangent(b);
    __BC_PRECONDITION_CONVERT(tangentB==BC_FLOAT_LARGE,BCErrorCubicMake(BCErrorArg1));
    //since BCCubicMakeConnectingTangents also uses rvalue BCErrorCubic, pass it through
    return BCCubicMakeConnectingTangents(connecting, bc_make_float2(tangentA, tangentB),bc_make_float2(distance,distance));
}

__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:tangentMagnitudes:)")))
/**
 \throws rvalue is \c BCErrorCubic.
 */
static inline BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b,bc_float2_t tangentMagnitudes) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b.a;
    //UB checked inside BCCubicInitialTangent / BCCubicFinalTangent, respectively
    //need to reverse b's initial tangent
    const bc_float2_t tangents = bc_make_float2(BCCubicFinalTangentAngle(a), BCCubicInitialTangentAngle(b));
    //since BCCubicMakeConnectingTangents also uses rvalue BCErrorCubic, pass it through
    return BCCubicMakeConnectingTangents(connecting, tangents, tangentMagnitudes);
}

///Specifies how tangent magnitudes are to be implied.  Can apply to both tangents or a single tangent, depending on context
typedef enum {
    ///Tangent magnitude should be copied from an input parameter
    BCTangentMagnitudeRuleCopied __attribute__((swift_name("copied"))),
    ///Tangent magnitude should be half the euclidian distance of the cubic, e.g. r/2
    BCTangentMagnitudeRuleHalfEuclidianDistance __attribute__((swift_name("halfEuclidianDistance"))),
} __attribute__((enum_extensibility(closed))) BCTangentMagnitudeRule;

///Creates a cubic connecting two cubics, with an initialTangent [finalTangent of the a] and finalTangent [reversed initialTangent of B]
///\seealso BCCubicMakeconnectingCubics for a version with explicit tangent magnitudes
///\warning This operation requires the curve to be technically normalized, see \c BCCubicNormalize
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:tangentRule:)")))
static inline BCCubic BCCubicMakeConnectingCubicsWithTangentRule(BCCubic a, BCCubic b,BCTangentMagnitudeRule tangentRule) {
    bc_float2_t lengths;
    switch (tangentRule) {
        case BCTangentMagnitudeRuleCopied:
        lengths = bc_make_float2(BCCubicInitialTangentMagnitude(a), BCCubicInitialTangentMagnitude(b));
        break;
        case BCTangentMagnitudeRuleHalfEuclidianDistance: {
            const bc_float_t distance = bc_distance(a.b, b.a) / 2;
            lengths = bc_make_float2(distance,distance);
                break;
        }
    }
    return BCCubicMakeConnectingCubics(a, b, lengths);
}




/**Creates a cubic connecting a cubic to a point, with an initialTangent [finalTangent of the a] and the finalTangent provided.
\discussion This will copy the tangent magnitudes of the initial cubic into the resulting cubic.  The magnitude of the cubic's \c finalTangent is determined by the \c r/2 rule.
\param a Initial cubic
\param b point to connect to
\param finalTangentAngle angle of the final tangent
\param initialTangentRule Rule to generate the initialTangent magnitude.  The \c finalTangentMagnitude will be \c BCTangentMagnitudeRuleHalfEuclidianDistance.
\throws rvalue is \c BCCubicError
 */
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:finalTangent:initialTangentRule:)")))
static inline BCCubic BCCubicMakeConnectingCubicToPoint(BCCubic a, bc_float2_t b, bc_float_t finalTangentAngle, BCTangentMagnitudeRule initialTangentRule) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b;
    bc_float2_t lengths;
    lengths.y = BCLineLength(connecting)/2;
    switch (initialTangentRule) {
    case BCTangentMagnitudeRuleCopied:
        lengths.x = BCCubicFinalTangentMagnitude(a);
        break;
    case BCTangentMagnitudeRuleHalfEuclidianDistance:
        lengths.x = lengths.y;
        break;
    }
    //since BCCubicMakeConnectingTangents also uses BCCubicError, pass it through.
    return BCCubicMakeConnectingTangents(connecting, bc_make_float2(BCCubicFinalTangentAngle(a), finalTangentAngle), lengths);
}

/**Creates a cubic by connecting a given line.  The points on the cubic should be the same as the points on the line.
\seealso \c BCCubicMakeConnectingTangents
 \throws BCCubicError if needed
 */
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:)")))
static inline BCCubic BCCubicMakeWithLine(BCLine a) {
    BCCubic out;
    out.a = a.a;
    out.b = a.b;
    out.c = BCLineEvaluate(a, 0.49);
    out.d = BCLineEvaluate(a, 0.51);
    __BC_BUGASSERT(!(out.c.x == BC_FLOAT_LARGE && out.c.y == BC_FLOAT_LARGE), BCErrorCubicMake(BCErrorLogic));
    __BC_BUGASSERT(!(out.d.x == BC_FLOAT_LARGE && out.d.y == BC_FLOAT_LARGE), BCErrorCubicMake(BCErrorLogic));
    return out;
}

/**\abstract Calculates the arclength.
This is a fast numerical and vectorized approximation for the arc length.  On some platforms this may produce different results.
\performance O(1)
*/
__attribute__((const))
__attribute__((swift_name("getter:Cubic.length(self:)")))
bc_float_t BCCubicLength(BCCubic c);


///Splits the curve at a given bezier parameter
///\performance O(1)
__attribute__((const))
__attribute__((swift_name("Cubic.split(self:t:)")))
BCCubic2 BCCubicSplit(BCCubic c, bc_float_t t);

///Calculates the "left" split of the cubic.
///\performance O(1).  This is faster than \c BCCubicSplit if you only need one half.  If you need both halves, it's faster to use \c BCCubicSplit.
__attribute__((const))
__attribute__((swift_name("Cubic.leftSplit(self:t:)")))
BCCubic BCCubicLeftSplit(BCCubic c, bc_float_t t);

///Calculates the "right" split of the cubic.
///\performance O(1).  This is faster than\c BCCubicSplit if you only need one half.  If you need both halves, it's faster to use \c BCCubicSplit.
__attribute__((const))
__attribute__((swift_name("Cubic.rightSplit(self:t:)")))
BCCubic BCCubicRightSplit(BCCubic c, bc_float_t t);

/**
 Performs an arclength parameterization.  This finds a bezier parameter \c t (in range 0,1) that is a length specified from \c cubic.a.
\performance We use an iterative approach.  Passing a higher value for \c threshold will let us stop earlier.
 \throws Checks arguments with assert.  rvalue is \c (-1-BCError).
 */
__attribute__((const))
__attribute__((swift_name("Cubic.parameterization(self:arclength:lowerBound:upperBound:threshold:)")))
bc_float_t BCCubicArclengthParameterizationWithBounds(BCCubic cubic, bc_float_t length, bc_float_t lowerBound, bc_float_t upperBound, bc_float_t threshold);

///Performs an arclength parameterization.  This finds a bezier parameter \c t (in range 0,1) that is a length specified from \c cubic.a.
///\performance We use an iterative approach.  Passing a higher value for \c threshold will let us stop earlier.
__attribute__((const))
__attribute__((swift_name("Cubic.parameterization(self:arclength:threshold:)")))
static inline bc_float_t BCCubicArclengthParameterization(BCCubic cubic, bc_float_t length, bc_float_t threshold) {
    return BCCubicArclengthParameterizationWithBounds(cubic, length, 0, 1, threshold);
}

/**Determines if the given cubic is normalized with a method appropriate for curvature calculations.
 \discussion See the documentation for \c BCCubicNormalize and \c BCNormalizationDistanceForCubicCurvatureError
 \seealso see \c BCAlignedCubicIsNormalizedForCurvature, a variant for the BCAlignedCubic case.
 @param cubic a cubic to check
 @param straightAngle Positive angle that is "nearly straight", used to detect the undesired curvature.  Plausible values for this parameter include \c 2*PI/360f
 @param curvatureError  maximum error (difference in unsigned curvature for a "nearly straight" cubic) we want.  I think this function is not well-behaved if you pass a value below \c 1*10^-14 or so, so if you intended to do that, just use \c BCLineLength(BCCubicAsLine(cubic))/2 rather than calling this function.
 
 @returns whether the cubic is normalized in a way appropriate for curvature calculations.
 */
__attribute__((const))
__attribute__((swift_name("Cubic.isNormalizedForCurvature(self:straightAngle:curvatureError:)")))
bool BCCubicIsNormalizedForCurvature(BCCubic cubic, bc_float_t straightAngle, bc_float_t curvatureError)
__attribute__((diagnose_if(!(straightAngle>0), "Invalid straightAngle","error")))
__attribute__((diagnose_if(!(curvatureError>0), "Invalid curvatureError","error")));


#endif

