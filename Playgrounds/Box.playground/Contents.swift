
import SwiftUI
import PlaygroundSupport
import blitcurve_c
import blitcurve

let box = Box(a: SIMD2<Float>(0,0), b: SIMD2<Float>(0,100),c: SIMD2<Float>(100,100))
PlaygroundPage.current.needsIndefiniteExecution = true
struct HoverView:UIViewRepresentable {
    var hoverCallback: ((CGPoint) -> Void)

    func makeUIView(context: UIViewRepresentableContext<HoverView>) -> UIView {
        let v = UIView(frame: .zero)
        v.isUserInteractionEnabled = true
        //you might think it makes sense to use 'hover' here, however that does not work due to FB7860081
        let gesture = UITapGestureRecognizer(target: context.coordinator, action: #selector(Coordinator.hovered))
        v.addGestureRecognizer(gesture)
        return v
    }

    class Coordinator: NSObject {
        var hoverCallback: ((CGPoint) -> Void)
        init(hoverCallback: @escaping ((CGPoint) -> Void)) {
            self.hoverCallback = hoverCallback
        }
        @objc func hovered(gesture:UIHoverGestureRecognizer) {
            let point = gesture.location(in: gesture.view)
            self.hoverCallback(point)
        }
    }

    func makeCoordinator() -> HoverView.Coordinator {
        return Coordinator(hoverCallback:self.hoverCallback)
    }

    func updateUIView(_ uiView: UIView,
                       context: UIViewRepresentableContext<HoverView>) {
    }

}

 
struct PointInsideChecker: View {
    let box: Box
    @State var hoverLocation: CGPoint?
    init(box: Box) {
        self.box = box
    }
    var body: some View {
        ZStack {
            Image(uiImage: box.playgroundDescription as! UIImage)
            if let hoverLocation = hoverLocation {
                let inside = box.isPointOnOrInside(SIMD2<Float>(Float(hoverLocation.x), Float(hoverLocation.y)))
                let circleColor: Color = inside ? .red : .blue
                Circle().offset(hoverLocation).size(width: 5, height: 5).foregroundColor(circleColor)
            }
            HoverView { (point) in
                self.hoverLocation = point
            }
        }
        
    }
}

PlaygroundPage.current.setLiveView(PointInsideChecker(box: box))
