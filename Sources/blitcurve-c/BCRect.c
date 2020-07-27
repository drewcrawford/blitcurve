//BCRect.c: BCRect functions
// ©2020 DrewCrawfordApps LLC

#include "BCRect.h"
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

BC4Points BCRectGet4Points(BCRect r) {
    bc_float2_t halflengths = r.lengths / 2;
    //unclear to me why, but we need to flip y and x
    bc_float2_t halflengths_t = simd_make_float2(halflengths.y,halflengths.x);
    bc_float2_t _a = simd_make_float2(halflengths_t.x, -halflengths_t.y);
    bc_float2_t _b = simd_make_float2(halflengths_t.x,halflengths_t.y);
    bc_float2_t _c = simd_make_float2(-halflengths_t.x,halflengths_t.y);
    bc_float2_t _d = simd_make_float2(-halflengths_t.x, -halflengths_t.y);
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(r.angle),sin(r.angle)), simd_make_float2(-sin(r.angle),cos(r.angle)));
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
    simd_float2x2 magic = bc_make_2x2(simd_make_float2(cos(r.angle),sin(r.angle)), simd_make_float2(-sin(r.angle),cos(r.angle)));
    bc_float4_t a_b = simd_make_float4(simd_mul(magic, _a),simd_mul(magic, _b));
    bc_float2_t c = simd_mul(magic, _c);
    a_b += simd_make_float4(r.center,r.center);
    c += r.center;
    BC3Points out;
    out.a_b = a_b;
    out.c = c;
    return out;
}

__attribute__((const))
///This is an implementation detail of\c  BCRectIntersects
static float BCRectIntersectsMakePointsign(bc_float2_t testPoint, bc_float2_t edgePoint, bc_float2_t perpendicular) {
    bc_float2_t sub = testPoint - edgePoint;
    bc_float_t dot = simd_dot(perpendicular, sub);
    return simd_sign(dot);
}

__attribute__((const))
///This is an implementation detail of \c BCRectIntersects
///\param e_a This is a point to check on e.  Note that we don't check points that are part of the edge itself.
static bool BCRectIntersectsLineSeparates(bc_float2_t e_a,  BC4Points f, bc_float2_t edgePoint, bc_float2_t perpendicular) {
     
    //first, we calculate sign for e_a
    float sign = BCRectIntersectsMakePointsign(e_a, edgePoint, perpendicular);
    //ensure that all f points are on a different side
    if (BCRectIntersectsMakePointsign(f.a_b.lo, edgePoint, perpendicular) == sign) {
        return false;
    }
    if (BCRectIntersectsMakePointsign(f.a_b.hi, edgePoint, perpendicular) == sign) {
        return false;
    }
    if (BCRectIntersectsMakePointsign(f.c_d.lo, edgePoint, perpendicular) == sign) {
        return false;
    }
    if (BCRectIntersectsMakePointsign(f.c_d.hi, edgePoint, perpendicular) == sign) {
        return false;
    }
    return true;
}


bool BCRectIntersects(BC4Points e, BC4Points f) {
    
    //edges from a
    bc_float2_t edgePoint = e.a_b.lo; //needs to a be some point from the edge

    //edge from a to b
    bc_float2_t edge = e.a_b.lo - e.a_b.hi;
    bc_float2_t perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.c_d.lo, f, edgePoint, perpendicular)) {
        return false;
    }
    //edge from a to c
    edge = e.a_b.lo - e.c_d.lo;
    perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.a_b.hi, f, edgePoint, perpendicular)) {
        return false;
    }
    
    //edge from a to d
    edge = e.a_b.lo - e.c_d.hi;
    perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.a_b.hi, f, edgePoint, perpendicular)) {
        return false;
    }
    
    //edges from b
    edgePoint = e.a_b.hi;
    
    //edge from b to c
    edge = e.a_b.hi - e.c_d.lo;
    perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.a_b.lo, f, edgePoint, perpendicular)) {
        return false;
    }
    
    //edge from b to d
    edge = e.a_b.hi - e.c_d.hi;
    perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.a_b.lo, f, edgePoint, perpendicular)) {
        return false;
    }
    
    //edges from c
    edgePoint = e.c_d.lo;
    
    //edge from c to d
    edge = e.c_d.lo - e.c_d.hi;
    perpendicular = simd_make_float2(-edge.y, edge.x);
    if (BCRectIntersectsLineSeparates(e.a_b.lo, f, edgePoint, perpendicular)) {
        return false;
    }
    return true;
}
