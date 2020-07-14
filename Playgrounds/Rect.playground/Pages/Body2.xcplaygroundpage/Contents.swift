//: [Previous](@previous)
import blitcurve
import SwiftUI
import PlaygroundSupport
let b = Rect(a: SIMD2<Float>(11.93, 8.075), b: SIMD2<Float>(9.163, 11.93), c: SIMD2<Float>(10.84, 11.93))
let point = SIMD2<Float>(11.0, 10.0)
struct Demo: View {
    var body: some View {
        BStack {
            Rect.View(b)
            PointView("x", coordinate: point).foregroundColor(.red)
        }.scale(by: 10)
    }
}

let k = b.isPointOnOrInside(point)
print(k)

PlaygroundPage.current.setLiveView(Demo())
