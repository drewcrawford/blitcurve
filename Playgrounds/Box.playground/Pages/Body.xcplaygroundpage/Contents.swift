//: [Previous](@previous)
import blitcurve
import PlaygroundSupport
import SwiftUI
let box = Box(a: SIMD2<Float>(10.779889, 5.138446), b: SIMD2<Float>(9.106642, 5.061841), c: SIMD2<Float>(8.930564, 8.907812))

struct D: View {
    var body: some View {
        Box.View(box).scale(by: 25.0)
    }
}

PlaygroundPage.current.setLiveView(D())
//: [Next](@next)
