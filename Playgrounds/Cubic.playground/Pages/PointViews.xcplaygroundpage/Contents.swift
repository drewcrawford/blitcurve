//: [Previous](@previous)

import SwiftUI
import blitcurve
import PlaygroundSupport

let c = Cubic(a: SIMD2<Float>(0, 0), b: SIMD2<Float>(10, 10), c: SIMD2<Float>(3, 6), d: SIMD2<Float>(7,9))

struct D: View {
    @State var interval: Float = 0.5
    var body: some View {
        VStack {
            BStack {
                PointViews(cubic: c, eachT: interval)
            }
            .scale(by: 20)
            .frame(width: 300, height: 300, alignment: .topLeading)
            Slider(value: $interval, in: 0...1)
        }
        
    }
}
PlaygroundPage.current.setLiveView(D())
//: [Next](@next)
