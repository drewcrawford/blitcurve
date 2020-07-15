import SwiftUI
import PlaygroundSupport
import blitcurve

let mainRail = Cubic(connecting: Line(a: SIMD2<Float>(0,10), b: SIMD2<Float>(60,10)), initialTangent: 0, finalTangent: 0)
let b1Rail = Cubic(connecting: Line(a: SIMD2<Float>(10,20), b: SIMD2<Float>(10,0)), initialTangent: 0, finalTangent: 0)

let b1_t = b1Rail.parameterization(arclength: 10, threshold: 0.01)
let b1 = Rect(center: b1Rail.evaluate(t: b1_t), lengths: SIMD2<Float>(3.85, 1.675), angle: b1Rail.tangentAt(t: b1_t))

let b2Rail = Cubic(connecting: Line(a: SIMD2<Float>(40,0), b: SIMD2<Float>(40,20)), initialTangent: 0, finalTangent: 0)
let b2_t = b1Rail.parameterization(arclength: 10, threshold: 0.01)
let b2 = Rect(center: b2Rail.evaluate(t: b2_t), lengths:  SIMD2<Float>(3.85, 1.675), angle: b2Rail.tangentAt(t: b2_t))

struct PageView: View {
    var body: some View {
        BStack {
            Cubic.View(mainRail)
            Rect.View(b1).foregroundColor(.red)
            Cubic.View(b1Rail).foregroundColor(.orange)
            Cubic.View(b2Rail).foregroundColor(.green)
            Rect.View(b2).foregroundColor(Color.yellow)
        }.scale(by: 5)
    }
}

PlaygroundPage.current.setLiveView(PageView())
