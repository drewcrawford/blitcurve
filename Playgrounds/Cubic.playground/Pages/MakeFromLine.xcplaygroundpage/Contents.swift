//: [Previous](@previous)

import blitcurve
import SwiftUI
import PlaygroundSupport
struct D: View {
    var body: some View {
        BStack {
            Cubic.View(Cubic(connecting: Line(a: .zero, b: SIMD2<Float>(100,100))))
            Cubic.View(Cubic(connecting: Line(a: .zero, b: SIMD2<Float>(0,100)))).foregroundColor(Color.red)
        }
        .frame(width: 300,height: 300)
    }
}
PlaygroundPage.current.setLiveView(D())
//: [Next](@next)
