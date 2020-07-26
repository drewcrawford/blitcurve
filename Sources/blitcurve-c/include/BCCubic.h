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

///\abstract Returns the line from a to c.
///\warning In the case that \code a =~= c \endcode, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangentLine(self:)")))
static inline BCLine BCCubicInitialTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.c;
    return l;
}

///\abstract Returns the line from d to b.
///\warning In the case that \code d=~= b \endcode, it may be difficult to use this sensibly
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangentLine(self:)")))
static inline BCLine BCCubicFinalTangentLine(BCCubic c) {
    BCLine l;
    l.a = c.d;
    l.b = c.b;
    return l;
}

///\abstract Returns the angle of the intitalTangent.
///\warning This requires the cubic to be partially normalized between \c a/c, see \c BCCubicNormalize for details.
__attribute__((const))
__attribute__((swift_name("getter:Cubic.initialTangent(self:)")))
static inline bc_float_t BCCubicInitialTangent(BCCubic c) {
    //UB checked inside BCLineTangent (e.g. the intial line would be 0)
    return BCLineTangent(BCCubicInitialTangentLine(c));
}

///\abstract Returns the angle of the finalTangent.
///\warning This requires the cubic to be partially normalized between \c b/d, see \c BCCubicNormalize for details.
__attribute__((const))
__attribute__((swift_name("getter:Cubic.finalTangent(self:)")))
static inline bc_float_t BCCubicFinalTangent(BCCubic c) {
    //UB checked inside BCLineTanget (e.g. the initial line would be 0)
    return BCLineTangent(BCCubicFinalTangentLine(c));
}

///\abstract Evaluate the cubic for the given bezier parameter
///\return A point on the cubic at the bezier equation for \c t
__attribute__((const))
__attribute__((swift_name("Cubic.evaluate(self:t:)")))
static inline bc_float2_t BCCubicEvaluate(BCCubic c,bc_float_t t) {
    return c.a * pow(1-t,3) + c.c * 3 * pow(1-t, 2) * t + c.d * 3 * (1 - t) * pow(t, 2) + c.b * pow(t, 3);
}
__attribute__((const))
__attribute__((swift_name("Cubic.evaluatePrime(self:t:)")))
///\abstract Evaluate the derivative of a BCCubic
///\return A point on the derivative of the given cubic at the given bezier parameter.
static inline bc_float2_t BCCubicEvaluatePrime(BCCubic c, bc_float_t t) {
    //D[a * (1-t)^3 + c * 3 * (1-t)^2 * t + d * 3 * (1 - t) * t^2 + b * t^3,t]
    //-3 a (1-t)^2+3 c (1-t)^2-6 c (1-t) t+6 d (1-t) t+3 b t^2-3 d t^2
    //-3 * c.a * pow(1-t,2) + 3 * c.c * pow(1-t,2) - 6 * c.c * (1-t) * t + 6 * c.d * (1 - t) * t + 3 * c.b * pow(t,2) - 3 * c.d * pow(t,2);
    const float one_minus_t = 1 - t;
    const float three_by_one_minus_t_squared = 3 * pow(one_minus_t, 2);
    const float three_by_t_squared = 3 * pow(t,2);
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
///In that situation, you can \c __BC_ASSERT(BCCubicIsNormalized(cubic)) instead.
static inline bool BCCubicIsTechnicallyNormalized(BCCubic c) {
    return simd_distance(c.c, c.a) != 0 && simd_distance(c.b, c.d)  != 0;
}



__attribute__((const))
__attribute__((swift_name("Cubic.tangentAt(self:t:)")))
///\abstract Evaluates the tangent at the given point
///\warning UB for non-normalized cubic
static inline bc_float_t BCCubicTangent(BCCubic c, bc_float_t t) {
    //check UB
    __BC_ASSERT(BCCubicIsTechnicallyNormalized(c));
    bc_float2_t prime = BCCubicEvaluatePrime(c, t);
    return atan2(prime.y, prime.x);
}



__attribute__((const))
__attribute__((swift_name("getter:Cubic.asLine(self:)")))
///\abstract Get a line from the \c BCCubic
static inline BCLine BCCubicAsLine(BCCubic c) {
    BCLine l;
    l.a = c.a;
    l.b = c.b;
    return l;
}
__attribute__((swift_name("Cubic.normalize(self:approximateDistance:)")))
///\abstract Normalize the cubic
///\discussion When a control point is exactly (or in some contexts, merely 'near') an endpoint, it creates various problems.  For example, the definition of an initial tangent is the angle between \c a and \c c, but if \c a=c this angle is undefined.  We call such difficult curves  \a non-normalized.
///
///We say a curve is \a partially normalized to refer to normalization between some subset of endpoints and control points that are required to mathematically define a particular operation.  For example, as discussed \c intialTangent requires a \a partial \a normalization for \c a and \c c.
///
///We say a curve is \a technically normalized if there is some nonzero distance between all related endpoints and control points.  This generally provides a mathematical definition for operations on the curve.
///
///In practice, applications may want a curve to be \a generously normalized.  For example, a small distance between \c a and \c c may technically define \c initialTangent at \c t=0, but the value is not necessarily close to the tangent at \c t=0.01, so an application scanning the bezier parameters at "reasonable" intervals may see weird behavior.  For this reason an application may want to normalize more generously to provide a larger distance between endpoints and control points.
///\param c The cubic to normalize.  We will normalize it in-place.
///\param approximateDistance the desired distance between endpoints and control points.  Pass a positive nonzero value.  Note the follwoing cases:
///1.  If the control/endpoints are already at least this distance, we will not alter them
///2.  If the control/endpoints are less than this distance, we will adjust the control points to a value approximately this distance, but it will not be exact, and the value we choose may be higher or lower.  For this reason, avoid values near the working precision, such as \c FLT_MIN, and avoid treating this as a minimum distance.
///\return A normalized curve, with at least the distance specified between related endpoints and control points.
static inline void BCCubicNormalize(BCCubic __BC_DEVICE *c, bc_float_t approximateDistance) {
    __BC_ASSERT(approximateDistance > 0);
    if (simd_distance(c->c, c->a) < approximateDistance) {
        const BCLine asLine = BCCubicAsLine(*c);
        bc_float_t t = BCLineArclengthParameterization(asLine, approximateDistance);
        c->c = BCLineEvaluate(asLine,t);
    }
    if (simd_distance(c->d, c->b) < approximateDistance) {
        const BCLine asLine = BCCubicAsLine(*c);
        bc_float_t t = 1 - BCLineArclengthParameterization(asLine, approximateDistance);
        c->d = BCLineEvaluate(asLine,t);
    }
}


__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:initialTangent:finalTangent:)")))
///Create a cubic that connects a line with initial and final tangents
///\warning This may be UBi if connecting is 0-len
static inline BCCubic BCCubicMakeConnectingTangents(BCLine connecting, bc_float_t initialTangent, bc_float_t finalTangent) {
    __BC_ASSERT(BCLineLength(connecting) > 0);
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
    //we subtract by M_PI here to reverse the finalTangent we were given
    bc_float2_t diff = simd_make_float2(initialTangent, finalTangent - M_PI);
    BCLine2 i_f = BCLine2MakeWithPointAndAngle(simd_make_float4(connecting.a,connecting.b),diff,simd_make_float2(1,1));
    c.c = i_f.b.xy;
    c.d = i_f.b.zw;
    return c;
}

///Creates a cubic connecting two cubics, with an initialTangent [finalTangent of the a] and finalTangent [reversed initialTangent of B]
///\warning This operation requires the curve to be technically normalized, see \c BCCubicNormalize
__attribute__((const))
__attribute__((swift_name("Cubic.init(connecting:to:)")))
static inline BCCubic BCCubicMakeConnectingCubics(BCCubic a, BCCubic b) {
    BCLine connecting;
    connecting.a = a.b;
    connecting.b = b.a;
    //UB checked inside BCCubicInitialTanget / BCCubicFinalTagent, respectively
    return BCCubicMakeConnectingTangents(connecting, BCCubicFinalTangent(a), BCCubicInitialTangent(b) + M_PI_2);
}

///\abstract Calculates the arclength.
///\note This is a fast numerical and vectorized approximation for the arc length.
///\performance O(1)
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

///Performs an arclength parameterization.  This finds a bezier parameter \c t (in range 0,1) that is a length specified from \c cubic.a.
///\performance We use an iterative approach.  Passing a higher value for \c threshold will let us stop earlier.
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

#endif

