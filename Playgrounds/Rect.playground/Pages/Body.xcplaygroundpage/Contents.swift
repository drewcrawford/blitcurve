//: [Previous](@previous)
import blitcurve
import SwiftUI
import PlaygroundSupport
let rect = Rect(center: SIMD2<Float>(10,10), lengths: SIMD2<Float>(1.675,3.85), angle:0)

struct D: View {
    var body: some View {
        BStack {
            Rect.View(rect)
            PointView("x",coordinate:SIMD2<Float>(11.925,10))
        }.scale(by: 10)
    }
}
PlaygroundPage.current.setLiveView(D())
//: [Next](@next)
