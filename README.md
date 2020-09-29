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
Intersecting rects at 4 gigaintersections/sec:

![intersect](https://raw.githubusercontent.com/wiki/drewcrawford/blitcurve/gifs/intersect.gif)

This demo is located [here](https://github.com/drewcrawford/blitcurve/Demos/RectIntersect).  For more details, see [this blogpost](https://sealedabstract.com/posts/rectangle-intersection/).

## Cubic drawing
24M paths per second

![intersect](https://raw.githubusercontent.com/wiki/drewcrawford/blitcurve/gifs/drawcubic.gif)
This demo is located [here](https://github.com/drewcrawford/blitcurve/Demos/LineShader)

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


For more examples, see the [Playgrounds](/Playgrounds) folder.


## Drawing curves

See the [LineShader](https://github.com/drewcrawford/blitcurve/Demos/LineShader) demo.
