import SwiftUI
import PlaygroundSupport
import blitcurve
let cubic = Cubic(a: SIMD2<Float>(192.0, 341.0), b: SIMD2<Float>(375.0, 667.0),c: SIMD2<Float>(188.91302, 335.85504),d: SIMD2<Float>(189.0, 336.0))

struct Demo: View {
    var body: some View {
        BStack {
            Cubic.View(cubic)
            PointView("t1",coordinate: cubic.evaluate(t: 0.10))
        }.scale(by: 0.5)
        .frame(width: 300, height: 300, alignment:.topLeading)
        
    }
}

PlaygroundPage.current.setLiveView(Demo())
