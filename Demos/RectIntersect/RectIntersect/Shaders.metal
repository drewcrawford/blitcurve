//
//  Shaders.metal
//  RectIntersect
//
//  Created by Drew Crawford on 8/3/20.
//

// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>
#include "ShaderTypes.h"

using namespace metal;

kernel void shapeSimulator(uint position[[thread_position_in_grid]], device const float2 *vectors, device BCRect *rects) {
    if (position >= RECT_COUNT) {
        return;
    }
    if (rects[position].lengths.x == 0 && rects[position].lengths.y == 0) {
        return;
    }
    rects[position].center += vectors[position];
}



vertex float4 vertexShader(uint instanceID [[instance_id]], uint vertexID [[vertex_id]], device const BCRect *rects)
{
    if (instanceID >= RECT_COUNT) {
        return simd_make_float4(2,2,2,2);
    }
    BCRect rect = rects[instanceID];
    //this is not a particularly optimized implementation
    BC4Points points = BCRectGet4Points(rect);
    if (vertexID == 0 || vertexID == 4) {
        return simd_make_float4(points.a_b.lo, simd_make_float2(0,1));
    }
    else if (vertexID == 1) {
        return simd_make_float4(points.a_b.hi, simd_make_float2(0,1));
    }
    else if (vertexID == 2) {
        return simd_make_float4(points.c_d.lo, simd_make_float2(0,1));
    }
    else if (vertexID == 3) {
        return simd_make_float4(points.c_d.hi, simd_make_float2(0,1));
    }
    
    else {
        __builtin_unreachable();
    }
}

fragment float4 fragmentShader(float4 in [[stage_in]])
{
    return simd_make_float4(1,1,1,1);
}
