//: [Previous](@previous)

import blitcurve
import SwiftUI
import PlaygroundSupport


let r1 = Cubic(connecting: Line(a: .zero, b: SIMD2<Float>(10,10)), initialTangent: 0, finalTangent: 0)
let r2 = Cubic(connecting: Line(a: SIMD2<Float>(10,10), b: SIMD2<Float>(20,20)), initialTangent: 0, finalTangent: 0)

let b1 = Rect(center: r1.evaluate(t: 1), lengths: SIMD2<Float>(1.675,3.85),angle: r1.tangentAt(t: 1))
struct D: View {
    var body: some View {
        BStack {
            Cubic.View(r1).foregroundColor(.red)
            Cubic.View(r2).foregroundColor(.blue)
            Rect.View(b1)
        }.scale(by: 10)
        .frame(width: 300, height: 300, alignment: .topLeading)
    }
    
}
PlaygroundPage.current.setLiveView(D())

//: [Next](@next)
