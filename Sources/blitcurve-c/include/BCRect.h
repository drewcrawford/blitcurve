//BCBox.h: Arbitrary 2D box
// ©2020 DrewCrawfordApps LLC
#ifndef BCBox_h
#define BCBox_h
#include "BCTypes.h"
#include "BCLine.h"
#ifndef __METAL_VERSION__
#include <stdbool.h>
#endif

__attribute__((swift_name("Rect")))
/**\abstract An arbitrary (non-axis-aligned) box.
\discussion This represents a rectangle, which is not necessarily axis-aligned (that is, it can be rotated to an arbitrary angle).
Since this type is guaranteed to be a rectangle, it can be defined on 3 points.
\see \c BCAlignedBox, for an axis-aligned type
 \code
                   a
                  ╱╲   angle
                 ╱  ╲ ▲
                ╱    ╳
               ╱    ╱ ╲
              ╱    ╱   ╲ b
             ╱    ╱    ╱
            ╱         ╱
           ╱  center ╱
          ╱         ╱
      d  ╱         ╱
         ╲        ╱lengths.y
          ╲      ╱
           ╲    ╱
 lengths.x  ╲  ╱
             ╲╱c
 \endcode
 */
typedef struct {
    ///The center of the rectangle
    bc_float2_t center;
    bc_float2_t lengths;
    bc_float_t angle;
    
} BCRect;

///4 arbitrary points, packed into a portable simd format
typedef struct {
    //a.x,a.y,b.x,b.y
    bc_float4_t a_b;
    //c.x,c.y,d.x,d.y
    bc_float4_t c_d;
} BC4Points;

///3 arbitrary points, packed into a portable simd format
typedef struct {
    //a.x,a.y,b.x,b.y
    bc_float4_t a_b;
    bc_float2_t c;
} BC3Points;

///\abstract Determines if the given point is on or inside the BCBox.
///\param b The 3-point representation.  These points \a must represent a rect (e.g. as calculated by \c BCRectGet3Points, points that do not represent a rect are undefined.
__attribute__((const))
__attribute__((swift_name("Rect.isPointOnOrInside(points:point:)")))
bool BCRectIsPointOnOrInside(BC3Points b, bc_float2_t point);


__attribute__((const))
__attribute__((swift_name("getter:Rect.points4(self:)")))
///\abstract Gets all 4 points of the rectangle.
///\discussion This will return point \c a in the upper-left (relative to the rect's angle).  \c b, \c c, and \c d will be in clockwise order (for an upper-right coordinate space).
BC4Points BCRectGet4Points(BCRect r);

__attribute__((const))
__attribute__((swift_name("getter:Rect.points3(self:)")))
///\abstract Gets 3 points of the rectangle.
///\discussion This will return point \c a in the upper-left (relative to the rect's angle).  \c b, \c c will be in clockwise order (for an upper-right coordinate space).
BC3Points BCRectGet3Points(BCRect r);



__attribute__((const))
__attribute__((swift_name("getter:Rect.max(self:)")))
///\abstract Find the maximum x/y coordinate in the box.
bc_float2_t  BCRectMax(BCRect b);



__attribute__((const))
__attribute__((swift_name("BCRect.intersects(self:_:)")))
///\abstract Calculates whether 2 \c BCRect intersect
bool BCRectIntersects(BCRect e, BCRect f);
#endif
