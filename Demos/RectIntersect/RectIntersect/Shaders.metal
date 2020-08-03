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



vertex float4 vertexShader(uint instanceID [[instance_id]], uint vertexID [[vertex_id]],
                               constant const uint16_t &instanceCount, device const BCRect *rects)
{
    if (instanceID > instanceCount) {
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
