//: [Previous](@previous)
import SwiftUI
import blitcurve
import PlaygroundSupport
struct D: View {
   @State var cubic = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(100,100), c: SIMD2<Float>(25,25), d: SIMD2<Float>(75,0))
    var body: some View {
        VStack {
            BStack {
                Cubic.View(cubic)
                PointView("m",coordinate: cubic.evaluate(t: AlignedCubic(cubic: cubic).maxKappaParameter))
            }
            .frame(width: 300, height: 300, alignment:.topLeading)
            GroupBox(label: Text("a")) {
                HStack {
                    Slider(value: $cubic.a.x, in: 0...100)
                    Text("\(cubic.a.x)")
                }
                HStack {
                    Slider(value: $cubic.a.y, in: 0...100)
                    Text("\(cubic.a.y)")
                }
            }
            GroupBox(label: Text("b")) {
                HStack {
                    Slider(value: $cubic.b.x, in: 0...100, step: 1)
                    Text("\(cubic.b.x)")
                    
                }
                HStack {
                    Slider(value: $cubic.b.y, in: 0...100)
                    Text("\(cubic.b.y)")
                }
            }
            GroupBox(label: Text("c")) {
                HStack {
                    Slider(value: $cubic.c.x, in: 0...100)
                    Text("\(cubic.c.x)")
                }
                HStack {
                    Slider(value: $cubic.c.y, in: 0...100)
                    Text("\(cubic.c.y)")
                }
            }
            GroupBox(label: Text("d")) {
                HStack {
                    Slider(value: $cubic.d.x, in: 0...100)
                    Text("\(cubic.d.x)")
                }
                HStack {
                    Slider(value: $cubic.d.y, in: 0...100)
                    Text("\(cubic.d.y)")
                }
            }
            GroupBox(label: Text("aligned")) {
                VStack {
                    Text("b.x \(AlignedCubic(cubic: cubic).b_x)")
                    Text("c \(AlignedCubic(cubic: cubic).c.x), \(AlignedCubic(cubic: cubic).c.y)")
                    Text("d \(AlignedCubic(cubic: cubic).d.x), \(AlignedCubic(cubic: cubic).c.y)")

                }
            }
        }
    }

}
PlaygroundPage.current.setLiveView(D())
PlaygroundPage.current.needsIndefiniteExecution = true
//: [Next](@next)
