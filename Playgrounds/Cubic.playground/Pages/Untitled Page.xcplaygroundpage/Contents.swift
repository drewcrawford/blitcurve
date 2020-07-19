import SwiftUI
import PlaygroundSupport
import blitcurve
let cubic = Cubic(a: .zero, b: SIMD2<Float>(20,10),c: SIMD2<Float>(7,7),d: SIMD2<Float>(5,5))

struct Demo: View {
    var body: some View {
        ZStack {
            Cubic.View(cubic).scale(by: 5)
        }
        
    }
}

PlaygroundPage.current.setLiveView(Demo())
