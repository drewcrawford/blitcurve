# blitcurve

A description of this package.


## Linking blitcurve (Xcode)

1.  Add a dependency on blitcurve.  File->Swift Packages->Add Package Dependencies...
2.  In your target settings, add blitcurve to your "Frameworks, Libraries, and Embedded Content".

### Linking the metal version

Blitcurve is cross-compiled: there's a C version and a metal shader version, so you can use it from inside metal shaders.
To add the metal shader version, you need to perform some additional steps.
1.  First, you need to add a build setting.  Scroll down to the "Metal Compiler - Build Options" heading and set `MTL_HEADER_SEARCH_PATHS` to be `${HEADER_SEARCH_PATHS}`.  
2.  Now you need to add some *custom* build settings.  You can do this by opening the project settings, and then choosing Edit -> Add Build Setting -> Add User Defined Setting.
    * Set `BLITCURVE_DIR` to the path to blitcurve on disk.  This is typically `${BUILD_ROOT}/../../SourcePackages/Checkouts/blitcurve` if your dependency is managed by Xcode.
    * Set `MTLLINKER_FLAGS` to `-L ${BUILT_PRODUCTS_DIR} -l blitcurve-metal`.  
    ![Example of settings](https://media.githubusercontent.com/media/drewcrawford/blitcurve/master/Documentation/images/build_settings.png)
    
3.  Create a "run script" phase for your target.  
    * The phase should be before the "Compile Sources" phase in your target settings.  You may need to drag it earlier in the list.
    * The phase should have the following contents:
        ```sh
        if [ -z ${BLITCURVE_DIR+x} ]; then 
        echo "error: set BLITCURVE_DIR"
        exit 1
        fi
        (
            . "${BLITCURVE_DIR}/makemetal.sh"
        )
        ```
    * The "Input file lists" should contain one entry, `${BLITCURVE_DIR}/inputfiles.txt`.  This improves incremental compile speeds.
    * The "Output file lists" should contain one entry, `${BLITCURVE_DIR}/outputfiles.txt`  This improves incremental compile speeds.
    ![Example of phase](https://media.githubusercontent.com/media/drewcrawford/blitcurve/master/Documentation/images/script_phase.png)
    

# Using blitcurve


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
