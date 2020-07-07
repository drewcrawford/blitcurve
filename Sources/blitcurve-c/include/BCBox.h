//BCBox.h: Arbitrary 2D box
// ©2020 DrewCrawfordApps LLC
#import "BCTypes.h"
#import "BCLine.h"
#ifndef __METAL_VERSION__
#import <stdbool.h>
#endif
///\abstract An arbitrary (non-axis-aligned) box.
///\discussion This represents a rectangle, which is not necessarily axis-aligned (that is, it can be rotated to an arbitrary angle).
///Since this type is guaranteed to be a rectangle, it can be defined on 3 points.
///\see \c BCAlignedBox, for an axis-aligned type
__attribute__((swift_name("Box")))
typedef struct {
    ///\abstract first point in the winding order
    bc_float2_t a;
    ///\abstract second point in the winding order
    bc_float2_t b;
    ///\abstract third point in the winding order
    bc_float2_t c;
    //note that the last point can be computed from these 3; as such it is not contained in the type.
} BCBox;

///\abstract Determines if the given point is on or inside the BCBox.
__attribute__((const))
__attribute__((swift_name("Box.isPointOnOrInside(self:_:)")))
bool BCBoxIsPointOnOrInside(BCBox b, bc_float2_t point);

__attribute__((const))
__attribute__((swift_name("Box.init(center:angle:lengths:)")))
/**\abstract Create a BCBox from a point and angle
\param center The centerpoint for this BCBox
\param angle The angle about which to rotate.  Each value in the vector will be treated as the length in that dimension.  See diagram.
\param lengths The lengths of the box.  Each value in the float2 maps to a dimension.  See the diagram
\code
                              
                 ╱╲   angle
                ╱  ╲ ▲
               ╱    ╳
              ╱    ╱ ╲
             ╱    ╱   ╲
            ╱    ╱    ╱
           ╱         ╱
          ╱  center ╱
         ╱         ╱
       ╲╱         ╱
        ╲        ╱lengths.y
         ╲      ╱
          ╲    ╱
lengths.x  ╲  ╱
            ╲╱
\endcode
 */
BCBox BCBoxMakeWithCenterAndAngle(bc_float2_t center, bc_float_t angle, bc_float2_t lengths);

__attribute__((const))
__attribute__((swift_name("getter:Box.d(self:)")))
///\abstract Calcuates the point D.
///\todo Possibly algorithms that need this value should be extracted to a different type to improve performance
static inline bc_float2_t BCBoxD(BCBox b) {
    BCLine dim2;
    dim2.a = b.b;
    dim2.b = b.c;
    bc_float_t angle = BCLineTangent(dim2);
    
    BCLine dim1;
    dim1.a = b.a;
    dim1.b = b.b;
    BCLine r = BCLineMakeWithPointAndAngle(b.c,angle - M_PI / 2, BCLineLength(dim1));
    return r.b;
}

__attribute__((const))
__attribute__((swift_name("getter:Box.max(self:)")))
///\abstract Find the maximum x/y coordinate in the box.
static inline bc_float2_t  BCBoxMax(BCBox b) {
    bc_float2_t d = BCBoxD(b);
    bc_float_t x = simd_reduce_max(simd_make_float4(b.a.x,b.b.x,b.c.x,d.x));
    bc_float_t y = simd_reduce_max(simd_make_float4(b.a.y,b.b.y,b.c.y,d.y));
    return simd_make_float2(x,y);
}
