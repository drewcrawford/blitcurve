//
//  RenderPass.metal
//  LineShader
//
//  Created by Drew Crawford on 9/28/20.
//

#include <metal_stdlib>
#include <blitcurve.h>
#include "ShaderTypes.h"
using namespace metal;
struct Vertex {
    float4 position [[position]];
    float3 color;
};
vertex Vertex vertexFunction(ushort vertexID [[vertex_id]], const uint instanceID [[instance_id]], BCCubic const device *cubics, ushort const constant &maxCubic, device const float3 *colors) {
    Vertex output;
    if (instanceID > maxCubic) {
        output.position = simd_float4(0,0,-2,-2);
        return output;
    }
    BCCubic cubic = cubics[instanceID];
    
    float3 o = BCCubicVertexMake(cubic, vertexID, VertexesPerCubic, simd_float3x3(simd_float3(1,0,0),simd_float3(0,1,0),simd_float3(0,0,1)));
    output.position = bc_make_float4(o.x,o.y,o.z,1);
    output.color = colors[instanceID];
    return output;
   
}
[[early_fragment_tests]]
fragment float4 fragmentFunction(Vertex in [[stage_in]]) {
    return float4(in.color.x,in.color.y,in.color.z,1);
}

