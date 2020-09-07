//BCBezierParameter.h: Bezier parameter functions
// ©2020 DrewCrawfordApps LLC
#ifndef BCBezierParameter_h
#define BCBezierParameter_h
#include "BCTypes.h"
#include "BCMacros.h"

/**\abstract Converts between a vertex ID and a bezier parameter.
\param vertexID The vertex id, e.g. a shader parameter.  We provide overloadable variants for \c uint16_t and \c uint32_t.  This must be \c <vertexesPerInstance.
 \param vertexesPerInstance The number of vertexes to draw for each instance.
@see BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT(vertexID < vertexesPerInstance);
    __BC_ASSERT(vertexesPerInstance > 1);
    return ((float)vertexID) / (vertexesPerInstance - 1);
}

/**
\abstract Converts between a vertex ID and a bezier parameter, with a custom min and max \c t.
\discussion This is primarily useful for the case that you want to evaluate points in a region of the curve, but without splitting the curve.  Splitting the curve is generally more efficient if the split can be used across multiple points, but that would require additional memory.
\param vertexID The vertex id, e.g. a shader parameter.
\param vertexesPerInstance The number of vertexes to draw for each instance.
\param lowerT The return value will be \c >= this number
\param upperT The return value will be \c <= this number.
 @see \c BCLUTParameterForIndex for non-graphical use
 */
__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("VertexToBezierParameter(vertexID:vertexesPerInstance:lowerT:upperT:)")))
inline bc_float_t BCVertexToBezierParameter(uint16_t vertexID, unsigned char vertexesPerInstance, float lowerT, float upperT)
__attribute__((diagnose_if(!(vertexID < vertexesPerInstance), "vertexID out of range","error")))
__attribute__((diagnose_if(!(lowerT < upperT), "bezier upper/lower out of range","error")))
__attribute__((diagnose_if(!(vertexesPerInstance > 1), "vertexesPerInstance out of range","error")))
{
    __BC_ASSERT(vertexID < vertexesPerInstance);
    __BC_ASSERT(lowerT < upperT);
    __BC_ASSERT(vertexesPerInstance > 1);
    float range = upperT - lowerT;
    return ((float)vertexID) / (vertexesPerInstance - 1) * range + lowerT;
}

///MARK: Provides a variety of methods for packing bezier parameters into a LUT (look-up table)

__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("LUTParameter(index:LUTCapacity:lowerT:upperT:)")))
/**
 Calculates which bezier parameter to use in an LUT of given size.
 @seealso BCVertexToBezierParameter,  a variant for graphical use
 @seealso This is the inverse of \c BCLUTIndexForParameter
 @seealso The overload without \c lowerT or \c upperT, if you want a value on the range 0,1
 @discussion This variant is generally useful in the case that you want to LUT on a region of the curve without splitting.  Splitting the curve may be more efficient.
 @param lutIndex index into the LUT
 @param lutCapacity total capacity of the LUT
 @param lowerT lower bound of the curve.
 @param upperT upper bound of the curve
 @returns a bezier parameter on the interval \c [lowerT,upperT] suitable for storing in a LUT of \c lutCapacity at index \c lutIndex.  The LUT is equally-weighted for any bezier parameter.
 */
inline bc_float_t BCLUTParameterForIndex(uint8_t lutIndex, uint8_t lutCapacity, float lowerT, float upperT)
__attribute__((diagnose_if(!(lowerT < upperT), "bezier upper/lower out of range","error")))
__attribute__((diagnose_if(!(lutIndex < lutCapacity), "lutIndex out of bounds","error")))
__attribute__((diagnose_if(!(lutCapacity>1), "lutCapacity too small","error")))

{
    __BC_ASSERT(lowerT < upperT);
    __BC_ASSERT(lutIndex < lutCapacity);
    __BC_ASSERT(lutCapacity>1);
    float range = upperT - lowerT;
    return ((float) lutIndex / (lutCapacity - 1) * range + lowerT);
}

__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("LUTParameter(index:capacity:)")))
/**
 Calculates which bezier parameter to use for a LUT of given size.  This version assumes a parameter in the range 0,1
 @seealso Compare with the variant with \c lowerT or \c upperT, to build a LUT for a given range of parameters.
 @seealso This is the inverse of \c BCLUTIndexForParameter
 @seealso Compare with BCVertexToBezierParameter,  a variant for graphical use
 
 @param lutIndex index into the LUT
 @param lutCapacity total capacity of the LUT
 @returns a bezier parameter on the interval \c [0,1] suitable for storing in a LUT of \c lutCapacity at index \c lutIndex.  The LUT is equally-weighted for any bezier parameter.
 */
inline bc_float_t BCLUTParameterForIndex(uint8_t lutIndex, uint8_t lutCapacity)
__attribute__((diagnose_if(!(lutIndex < lutCapacity), "lutIndex out of range","error")))
__attribute__((diagnose_if(!(lutCapacity>1), "lutCapacity too small","error")))
{
    __BC_ASSERT(lutIndex < lutCapacity);
    __BC_ASSERT(lutCapacity>1);
    return ((float) lutIndex / (lutCapacity - 1));
}

///Specifies how \c t that do not exactly fall into an LUT are to be mapped to indexes
__attribute__((swift_name("LUTRoundingMode")))
typedef enum {
    ///Pick the closest index
    BCLUTRoundingModeClosest __attribute__((swift_name("closest"))),
    ///Pick the smaller of the two indexes
    BCLUTRoundingModeLeast __attribute__((swift_name("least"))),
    ///Pick the larger of the two indexes
    BCLUTRoundingModeMost __attribute__((swift_name("most"))),
} __attribute__((enum_extensibility(closed))) BCLUTRoundingMode;

__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("LUTIndex(t:capacity:roundingMode:lowerT:upperT:)")))
/**Finds an index into a LUT for the given parameter.
 @seealso Compare with the variant without \c lowerT/upperT
 @seealso This is the inverse of \c BCLUTParameterForIndex
 @discussion This is generally useful in the case that you want to LUT on a region of the curve without splitting.  Splitting the curve may be more efficient.
 @param t A bezier parameter on range \c [lowerT,upperT]
 @param lutCapacity total capacity of the LUT
 @param roundingMode The rounding mode to use when converting a parameter to an index.  For most cases, \c BCLUTRoundingModeClosest is appropriate and will choose the closest index to the parameter.  If the LUT comprises a minimum or maximum bounds, a different mode might be appropriate.
 @param lowerT lower bound of the curve.
 @param upperT upper bound of the curve.
 @returns A LUT index for for a table with \c lutCapacity, table covering the curve range \c [lowerT,upperT].  The LUT is equally-weighted for any bezier parameter.
 */
inline uint8_t BCLUTIndexForParameter(float t, uint8_t lutCapacity, BCLUTRoundingMode roundingMode, float lowerT, float upperT)
__attribute__((diagnose_if(!(lowerT < upperT), "bezier upper/lower out of range","error")))
__attribute__((diagnose_if(!(t >= lowerT && t <= upperT), "t out of range","error")))
__attribute__((diagnose_if(!(lutCapacity>1), "LUT too small","error")))
__attribute__((diagnose_if(!(roundingMode <= BCLUTRoundingModeMost), "Invalid rounding mode","error")))
{
    __BC_ASSERT(lowerT < upperT);
    __BC_ASSERT(t >= lowerT && t <= upperT);
    __BC_ASSERT(lutCapacity>=1);
    __BC_ASSERT(roundingMode <= BCLUTRoundingModeMost);
    float indexf = (t - lowerT) * (lutCapacity - 1) * (1 / (upperT - lowerT));
    switch (roundingMode) {
        case BCLUTRoundingModeClosest:
            return roundf(indexf);
        case BCLUTRoundingModeMost:
            return ceilf(indexf);
        case BCLUTRoundingModeLeast:
            return floorf(indexf);
    }
    __builtin_unreachable();
}

__attribute__((const))
__attribute__((overloadable))
__attribute__((swift_name("LUTIndex(t:capacity:roundingMode:)")))
/**Finds an index into a LUT for the given parameter.
 @seealso This is the inverse of \c BCLUTParameterForIndex
 @seealso Compare with the variant with \c lowerT/upperT
 @param t A bezier parameter on range \c [0,1]
 @param lutCapacity total capacity of the LUT
 @param roundingMode The rounding mode to use when converting a parameter to an index.  For most cases, \c BCLUTRoundingModeClosest is appropriate and will choose the closest index to the parameter.  If the LUT comprises a minimum or maximum bounds, a different mode might be appropriate.
 @returns A LUT index for for a table with \c lutCapacity, table covering the curve range \c [0,1].  The LUT is equally-weighted for any bezier parameter.
 */
inline uint8_t BCLUTIndexForParameter(float t, uint8_t lutCapacity, BCLUTRoundingMode roundingMode)
__attribute__((diagnose_if(!(t>=0 && t <=1), "parameter out of range","error")))
__attribute__((diagnose_if(!(lutCapacity>1), "LUT too small","error")))
__attribute__((diagnose_if(!(roundingMode <= BCLUTRoundingModeMost), "invalid rounding mode","error")))
{
    __BC_ASSERT(t >= 0 && t <= 1);
    __BC_ASSERT(lutCapacity>1);
    __BC_ASSERT(roundingMode <= BCLUTRoundingModeMost);
    float indexf = t * (lutCapacity - 1);
    switch (roundingMode) {
        case BCLUTRoundingModeClosest:
            return roundf(indexf);
        case BCLUTRoundingModeMost:
            return ceilf(indexf);
        case BCLUTRoundingModeLeast:
            return floorf(indexf);
    }
    __builtin_unreachable();
}
#endif
