import SwiftUI
import PlaygroundSupport
import blitcurve

let b = Rect(center: SIMD2<Float>(50,50), lengths: SIMD2<Float>(10,10), angle: 0)
let b1 = Rect(center: SIMD2<Float>(100,100), lengths: SIMD2<Float>(10,10), angle: 0)

struct PageView: View {
    var body: some View {
        BStack {
            Rect.View(b)
            Rect.View(b1)
        }
        
    }
}

PlaygroundPage.current.setLiveView(PageView())
