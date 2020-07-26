import blitcurve
import SwiftUI
import PlaygroundSupport

let l = Line(a: SIMD2<Float>(10,50), b: SIMD2<Float>(20,20))
//let l = Line(a: SIMD2<Float>(0,0), b: SIMD2<Float>(50,50))

struct D: View {
    var body: some View {
        BStack {
            Line.View(l)
        }.scale(by: 1)
        .frame(width: 300, height: 300, alignment: .topLeading)
    }
}

PlaygroundPage.current.setLiveView(D())
