# blitcurve

blitcurve is a C/C++ library for bezier curves and similar 2D geometric primitives, including common algorithms.  
These mathematical objects have applications in games, animations, physical behaviors, simulations, etc. 

blitcurve has a first-class Swift API for use in iOS and macOS applications.  It also cross-compiles for Metal and can be used entirely on-GPU, in which case it is many orders-of-magnitude faster than alternative bezier curve approaches.

![status](https://img.shields.io/badge/status-alpha-red) ![xcode](https://img.shields.io/badge/xcode-12-blue) ![platforms](https://img.shields.io/badge/platforms-ios%20%7C%20macOS-lightgrey) ![license](https://img.shields.io/badge/license-noncommercial-important)



# Installation
See the [Installation](https://github.com/drewcrawford/blitcurve/wiki/Installation) page.

# Using blitcurve

For more examples, see the [Wiki](https://github.com/drewcrawford/blitcurve/wiki/Installation) and the [Playgrounds](Playgrounds) directory.

# Demos
## Rect intersection
<video controls muted width="800">
    <source src="https://github.com/drewcrawford/blitcurve/videos/intersection/demo-vp9-30.webm" type='video/webm; codecs="vp9"'>
    <source src="https://github.com/drewcrawford/blitcurve/videos/intersection/demo-h265-40-195.mp4" type='video/mp4; codecs="hvc"'>
    <source src="https://github.com/drewcrawford/blitcurve/videos/intersection/demo-h264-41-20.mp4" type='video/mp4; codecs="avc1.640029"'>
</video>

This demo is located [here](https://github.com/drewcrawford/blitcurve/demos/RectIntersect).  For more details, see [this blogpost](https://sealedabstract.com/posts/rectangle-intersection/).

## SwiftUI 

Many types embed a `View` type which is a SwiftUI view.

```swift
let cubic = Cubic(a: .zero, b: SIMD2<Float>(20,10),c: SIMD2<Float>(7,7),d: SIMD2<Float>(5,5))
struct Demo: View {
    var body: some View {
        ZStack {
            Cubic.View(cubic).scale(by: 5)
        }
        
    }
}
```


For more, see the [Playgrounds](/Playgrounds) folder.


## Drawing curves

Assuming some familiarity with metal, here is an example of how to draw an array of BCCubic.

You may be familiar with [prior work on curve flattening](https://raphlinus.github.io/graphics/curves/2019/12/23/flatten-quadbez.html). We use the same general idea, however
1.  In this example, we use a constant number of vertices per cubic, rather than determining per-curve.  In practice, I often want to relate this number to the transform.
2.  We send the whole curve down to the GPU, and do the 'flattening' in a vertex shader.

```swift
func draw(in encoder: MTLRenderCommandEncoder, transform: float3x3) {
    encoder.setRenderPipelineState(pipelineState)
    
    encoder.setVertexBytes(v: transform, index: 0)
    let vertexesPerCubic: UInt8 = 15
    encoder.setVertexBytes(v: vertexesPerCubic, index: 1)
    encoder.setVertexBuffers([cubicBuffer], range: 2..<3)
    encoder.drawPrimitives(type: .lineStrip, vertexStart: 0, vertexCount: Int(vertexesPerCubic), instanceCount: cubicBuffer.count)
}
```

```metal
#import <blitcurve.h>
using namespace metal;
typedef struct  {
    float4 position [[position]];
} VertexOutput;
vertex VertexOutput VertexShader(ushort vertexID [[vertex_id]], ushort instanceID [[instance_id]], float3x3 constant &transform, uint8_t constant &vertexesPerCubic, BCCubic device *cubics) {
    VertexOutput output;
    BCCubic cubic = cubics[instanceID];
    
    output.position = simd_float4(0, 0, 1, 1);
    output.position.xy = BCCubicEvaluate(cubic, BCVertexToBezierParameter(vertexID, vertexesPerCubic));
    output.position.xyz = output.position.xyz * transform;
    return output;
   
}
fragment float4 VertexShader(VertexOutput f [[stage_in]]) {
    return simd_float4(1,1,1,1);
}
```
