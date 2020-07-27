//: [Previous](@previous)
import blitcurve
import SwiftUI
import PlaygroundSupport

let r = Rect(center: SIMD2<Float>(10,10), lengths: SIMD2<Float>(1,1), angle: 0)
let r2 = Rect(center: SIMD2<Float>(15,15), lengths: SIMD2<Float>(1,1), angle: 0)

struct R: View {
    var body: some View {
        BStack {
            Rect.View(r)
            Rect.View(r2)
        }
        .scale(by: 30)
        .frame(width: 300, height: 300, alignment: .topLeading)
    }
}

PlaygroundPage.current.setLiveView(R())
//: [Next](@next)
