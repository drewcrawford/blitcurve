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



kernel void shapeSimulator(ushort position[[thread_position_in_grid]], device const float2 *vectors, device uint8_t *intersects, device BCRect *rects) {
    if (position >= RECT_COUNT) {
        return;
    }
    if (rects[position].lengths.x == 0 && rects[position].lengths.y == 0) {
        return;
    }
    
    rects[position].center += vectors[position];
    
    BCRect rect = rects[position];
    //intersection code
    intersects[position] = 0;
    for (ushort i = 0; i < RECT_COUNT; i++) {
        if (i==position) { continue; }
        BCRect testRect = rects[i];
        if (BCRectIntersects(rect, testRect)) {
            intersects[position] = 1;
            return;
        }
    }
}

struct Vertex {
    float4 position [[position]];
    bool intersection;
};


vertex Vertex vertexShader(ushort instanceID [[instance_id]], ushort vertexID [[vertex_id]], device const BCRect *rects, device const uint8_t *intersects)
{
    Vertex v;
    
    if (instanceID >= RECT_COUNT) {
        v.position = simd_make_float4(2,2,2,2);
    }
    BCRect rect = rects[instanceID];
    //this is not a particularly optimized implementation
    BC4Points points = BCRectGet4Points(rect);
    if (vertexID == 0 || vertexID == 4) {
        v.position = simd_make_float4(points.a_b.lo, simd_make_float2(0,1));
    }
    else if (vertexID == 1) {
        v.position = simd_make_float4(points.a_b.hi, simd_make_float2(0,1));
    }
    else if (vertexID == 2) {
        v.position = simd_make_float4(points.c_d.lo, simd_make_float2(0,1));
    }
    else if (vertexID == 3) {
        v.position = simd_make_float4(points.c_d.hi, simd_make_float2(0,1));
    }
    else {
        __builtin_unreachable();
    }
    v.intersection = intersects[instanceID];
    return v;
}

fragment float4 fragmentShader(Vertex in [[stage_in]])
{
    if (in.intersection) {
        return simd_make_float4(1, 0, 0, 1);
    }
    else {
        return simd_make_float4(1, 1, 1, 1);
    }
}
