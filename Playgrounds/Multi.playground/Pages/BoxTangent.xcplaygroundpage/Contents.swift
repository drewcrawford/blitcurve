//: [Previous](@previous)

import blitcurve
import SwiftUI
import PlaygroundSupport
let r0 = Cubic(a: SIMD2<Float>(58.95, 110), b: SIMD2<Float>(56.50, 105.7), c: SIMD2<Float>(58.20, 108.7), d: SIMD2<Float>(57.38, 107.2))

let bodySize = SIMD2<Float>(1.675, 3.85)

let t: Float = 0.2
let box = Rect(center: r0.evaluate(t: t), lengths:bodySize, angle: r0.tangentAt(t: t))
print(box)
print(box.points4)
struct D: View {
    var body: some View {
        BStack {
            Cubic.View(r0).foregroundColor(.blue)
            Rect.View(box)
        }.scale(by: 20)
        .frame(width: 300, height: 300, alignment:
                .topLeading)
    }
}

PlaygroundPage.current.setLiveView(D())


//: [Next](@next)
