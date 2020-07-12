import SwiftUI
import PlaygroundSupport
import blitcurve

let mainRail = Cubic(connecting: Line(a: SIMD2<Float>(0,10), b: SIMD2<Float>(60,10)), initialTangent: 0, finalTangent: 0)
let b1Rail = Cubic(connecting: Line(a: SIMD2<Float>(10,20), b: SIMD2<Float>(10,0)), initialTangent: 0, finalTangent: 0)

let b2Rail = Cubic(connecting: Line(a: SIMD2<Float>(40,0), b: SIMD2<Float>(40,20)), initialTangent: 0, finalTangent: 0)

struct PageView: View {
    var body: some View {
        BStack {
            Cubic.View(mainRail)
            Cubic.View(b1Rail)
            Cubic.View(b2Rail)
        }.scale(by: 8)
    }
}

PlaygroundPage.current.setLiveView(PageView())
