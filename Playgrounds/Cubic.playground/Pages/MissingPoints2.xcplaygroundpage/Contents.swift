//: [Previous](@previous)
import SwiftUI
import PlaygroundSupport
import blitcurve
//this test case involves a situation where we aren't displaying all the points
let r0 = Cubic(a: SIMD2<Float>(58.95, 110), b: SIMD2<Float>(56.50, 105.7), c: SIMD2<Float>(58.20, 108.7), d: SIMD2<Float>(57.38, 107.2))


struct D: View {
    var body: some View {
        BStack {
            Cubic.View(r0)
            //Cubic.View(r418).foregroundColor(.red)
            //Rect.View(box)
        }
        .scale(by: 20)
        .frame(width: 500, height: 500, alignment:
                .topLeading)
    }
}

PlaygroundPage.current.setLiveView(D())


//: [Next](@next)
