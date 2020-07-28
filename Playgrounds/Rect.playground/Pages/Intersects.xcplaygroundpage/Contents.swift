//: [Previous](@previous)
import blitcurve
import SwiftUI
import PlaygroundSupport
struct Inspector: View {
    let label: () -> Text

    let binding: Binding<Rect>
    var body: some View {
        GroupBox(label: label()) {
            HStack {
                Text("x")
                Slider(value: binding.center.x, in:10...20)
                
                Text("\(binding.center.x.wrappedValue)")
            }
            HStack {
                Text("y")
                Slider(value: binding.center.y, in:10...20)
                Text("\(binding.center.y.wrappedValue)")
            }
            HStack {
                Text("angle")
                Slider(value: binding.angle, in: 0...(.pi))
                Text("\(binding.angle.wrappedValue)")

            }
            HStack {
                Text("width")
                Slider(value: binding.lengths.x, in:0...5)
                Text("\(binding.lengths.x.wrappedValue)")
            }
            HStack {
                Text("height")
                Slider(value: binding.lengths.y, in: 0...5)
                Text("\(binding.lengths.y.wrappedValue)")
            }
        }
    }
}

struct R: View {
    @State var r1 = Rect(center: SIMD2<Float>(10,10), lengths: SIMD2<Float>(1,1), angle: 0)
    @State var r2 = Rect(center: SIMD2<Float>(15,15), lengths: SIMD2<Float>(1,1), angle: 0)
    var color: Color {
        if r1.intersects(r2) {
            return .red
        }
        return .black
    }
    var body: some View {
        VStack {
            BStack {
                Rect.View(r1)
                Rect.View(r2)
            }
            .scale(by: 30)
            .foregroundColor(color)
            .frame(width: 300, height: 200, alignment: .topLeading)
            Inspector(label: {Text("r1")}, binding: $r1)
            Inspector(label: {Text("r2")}, binding: $r2)

        }
        
    }
}

PlaygroundPage.current.setLiveView(R())
//: [Next](@next)
