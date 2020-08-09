import SwiftUI
import blitcurve
import PlaygroundSupport

let aRail = Cubic(a: SIMD2<Float>(2.6605175, 9.86255), b: SIMD2<Float>(0.21252254, 5.5065603), c: SIMD2<Float>(1.8522768, 8.424357), d: SIMD2<Float>(1.0362785, 6.9723616))
let nextRail = Cubic(a: SIMD2<Float>(4.6373005, 3.1232715), b: SIMD2<Float>(7.0850296, 7.478789), c: SIMD2<Float>(5.4609714, 4.5889215), d: SIMD2<Float>(6.276881, 6.04076))
let c = Cubic(a: SIMD2<Float>(0.213,5.507), b: SIMD2<Float>(4.637,3.123),c: SIMD2<Float>(-0.277,4.635), d: SIMD2<Float>(5.509,2.633))

let bodySize = SIMD2<Float>(1.675, 3.85)

let t: Float = 0.2
let box = Rect(center: c.evaluate(t: t), lengths:bodySize, angle: c.tangentAt(t: t))
struct D: View {
    var body: some View {
        BStack {

            Cubic.View(c)
            Rect.View(box)

            //Cubic.View(c)
//            Cubic.View(nextRail)
        }
        //.scale(by: 10)
        .frame(width: 300, height: 300, alignment: .topLeading)
    }
}

struct E: View {
    var body: some View {
        BStack {
            Cubic.View(c).foregroundColor(.blue)
            Rect.View(box)
        }.scale(by: 20)
        .frame(width: 300, height: 300, alignment:
                .topLeading)
    }
}
PlaygroundPage.current.setLiveView(D())
