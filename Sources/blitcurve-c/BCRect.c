//BCRect.c: BCRect functions
// ©2020 DrewCrawfordApps LLC

#include "BCRect.h"
bc_float2_t  BCRectMax(BCRect b) {
    BC4Points points = BCRectGet4Points(b);
    bc_float_t x_a = simd_reduce_max(points.a_b.even);
    bc_float_t x_b = simd_reduce_max(points.c_d.even);
    bc_float_t x_f = simd_max(x_a,x_b);

    bc_float_t y_a = simd_reduce_max(points.a_b.odd);
    bc_float_t y_b = simd_reduce_max(points.c_d.odd);
    bc_float_t y_f = simd_max(y_a,y_b);

    return simd_make_float2(x_f,y_f);
}

bool BCRectIsPointOnOrInside(BC3Points b, bc_float2_t point) {
    simd_float2 ab = b.a_b.hi - b.a_b.lo;
    simd_float2 am = point - b.a_b.lo;
    simd_float2 bc = b.c - b.a_b.hi;
    simd_float2 bm = point - b.a_b.hi;

    float abam = simd_dot(ab, am);
    float abab = simd_dot(ab, ab);
    float bcbm = simd_dot(bc, bm);
    float bcbc = simd_dot(bc, bc);

    if (0 <= abam && abam <= abab && 0 <= bcbm && bcbm <= bcbc) {
        return true;
    }
    return false;
}

__attribute__((const))
static simd_float2x2 BCRotationMatrix(float angle) {
    return bc_make_2x2(simd_make_float2(cos(angle),sin(angle)), simd_make_float2(-sin(angle),cos(angle)));
}

BC4Points BCRectGet4Points(BCRect r) {
    bc_float2_t halflengths = r.lengths / 2;
    //unclear to me why, but we need to flip y and x
    bc_float2_t halflengths_t = simd_make_float2(halflengths.y,halflengths.x);
    bc_float2_t _a = simd_make_float2(halflengths_t.x, -halflengths_t.y);
    bc_float2_t _b = simd_make_float2(halflengths_t.x,halflengths_t.y);
    bc_float2_t _c = simd_make_float2(-halflengths_t.x,halflengths_t.y);
    bc_float2_t _d = simd_make_float2(-halflengths_t.x, -halflengths_t.y);
    simd_float2x2 magic = BCRotationMatrix(r.angle);
    bc_float4_t a_b = simd_make_float4(simd_mul(magic,_a),simd_mul(magic, _b));
    bc_float4_t c_d = simd_make_float4(simd_mul(magic, _c),simd_mul(magic, _d));
    a_b += simd_make_float4(r.center,r.center);
    c_d += simd_make_float4(r.center,r.center);
    BC4Points out;
    out.a_b = a_b;
    out.c_d = c_d;
    return out;
}

BC3Points BCRectGet3Points(BCRect r) {
    bc_float2_t halflengths = r.lengths / 2;
    //unclear to me why, but we need to flip y and x
    bc_float2_t halflengths_t = simd_make_float2(halflengths.y,halflengths.x);
    bc_float2_t _a = simd_make_float2(halflengths_t.x, -halflengths_t.y);
    bc_float2_t _b = simd_make_float2(halflengths_t.x,halflengths_t.y);
    bc_float2_t _c = simd_make_float2(-halflengths_t.x,halflengths_t.y);
    simd_float2x2 magic = BCRotationMatrix(r.angle);
    bc_float4_t a_b = simd_make_float4(simd_mul(magic, _a),simd_mul(magic, _b));
    bc_float2_t c = simd_mul(magic, _c);
    a_b += simd_make_float4(r.center,r.center);
    c += r.center;
    BC3Points out;
    out.a_b = a_b;
    out.c = c;
    return out;
}

///\abstract implementation detail of \c BCRectIntersects
///\return \c false if we know there is no intersection, or \c true for inconclusive
static bool BCRectHalfIntersects(const BCRect fixed, BCRect movable) {
    /*Core idea here is that two rects don't intersect if a line can be drawn between them.
     
                       a
                      ╱ ╲
          │          ╱   ╲
          │         ╱     ╲
          │        ╱       ╲
    d─────┼───a┐  d   r2    b
    │     │    │   ╲       ╱
    │     │    │    ╲     ╱
    │   r1│    │     ╲   ╱
────┼─────┼────┼──▶   ╲ ╱
    │     │    │       c
    │     │    │
    │     │    │
    c─────┼────b
          │
          │
          │
     
"It follows easily" (because I don't know a proof) that such a line will be an edge of the rect.
     
Above, the line r1.ab and r2.dc separate.  Alternatively, in some cases only one line separates, such as is the case below
     
                  a
                 ╱ ╲
                ╱   ╲
               ╱     ╲
              ╱       ╲
          │  d   r2    b
          │   ╲       ╱
          │    ╲     ╱
    d─────┼───a┐╲   ╱
    │     │    │ ╲ ╱
    │     │    │  c
    │   r1│    │
────┼─────┼────┼──▶
    │     │    │
    │     │    │
    │     │    │
    c─────┼────b
          │
          │
          │
    
     Now, r2.dc separate, and no line in r1 separates.  For this reason we do need to check both rects.
     
     Now, how do we check such cases?  In the diagrams above, r1 is conveniently centered at the origin.
     In such cases, the edge lines have a convenient definition involving +/- halflength.  Further, we know
     (statically) which direction contains the r1, so we just check to see if the r2 point is on the opposite side.  We
     do this process for the 4 points in r2.
     
     How about cases where r1 is not at the origin?  Here we can transform the system so it is.  Since r1 is only used for the edge lines,
     which have a convenient defininition of +/- halflength, we don't bother to transform it.  For r2, we transform as follows
     
     * r2.angle -= r1.angle
     * r2.center -= r1.angle
     * r2.center.rotate(-r1.angle)
     
     Which should now be in the desired reference frame.  This is substantially faster than transforming the point-representation
     since we would have to apply transformations to each point.
     
     This is, as far as I know, the fastest way to do it, and is much better than other approaches I've found on the internet.
     Still, it would surprise me if this is the first implementation of it.
     */
    bc_float2_t halflengths = fixed.lengths / 2;
    movable.angle -= fixed.angle;
    movable.center -= fixed.center;
    movable.center = simd_mul(BCRotationMatrix(-fixed.angle), movable.center);
    
    BC4Points testPoints = BCRectGet4Points(movable);
    //swap all the x/y coordinates into a simd4
    //this is mostly to assist the compiler in vectorizing if it would like
    bc_float4_t xPoints = simd_make_float4(testPoints.a_b.even, testPoints.c_d.even);
    bc_float4_t yPoints = simd_make_float4(testPoints.a_b.odd, testPoints.c_d.odd);
    if (xPoints.x <= -halflengths.y && xPoints.y <= -halflengths.y && xPoints.z <= -halflengths.y && xPoints.w <= -halflengths.y ) {
        return false; //no intersection
    }
    if (xPoints.x >= halflengths.y && xPoints.y >= halflengths.y && xPoints.z >= halflengths.y && xPoints.w >= halflengths.y ) {
        return false; //no intersection
    }
    
    if (yPoints.x <= -halflengths.x && yPoints.y <= -halflengths.x && yPoints.z <= -halflengths.x && yPoints.w <= -halflengths.x ) {
        return false; //no intersection
    }
    if (yPoints.x >= halflengths.x && yPoints.y >= halflengths.x && yPoints.z >= halflengths.x && yPoints.w >= halflengths.x ) {
        return false; //no intersection
    }
    return true;
}
bool BCRectIntersects(BCRect e, BCRect f) {
    return BCRectHalfIntersects(e, f) && BCRectHalfIntersects(f, e);
}
