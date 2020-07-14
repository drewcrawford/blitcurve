
import SwiftUI
import PlaygroundSupport
import blitcurve

//note: This cannot be extracted due to some combination of FB7859836 and/or FB7935846 I believe
public struct HoverView:UIViewRepresentable {
    var hoverCallback: ((CGPoint) -> Void)
    
    public init(hoverCallback: @escaping ((CGPoint) -> Void)) {
        self.hoverCallback = hoverCallback
    }
    public func makeUIView(context: UIViewRepresentableContext<HoverView>) -> UIView {
        let v = UIView(frame: .zero)
        v.isUserInteractionEnabled = true
        //you might think it makes sense to use 'hover' here, however that does not work due to FB7860081
        let gesture = UITapGestureRecognizer(target: context.coordinator, action: #selector(Coordinator.hovered))
        v.addGestureRecognizer(gesture)
        return v
    }

    public class Coordinator: NSObject {
        var hoverCallback: ((CGPoint) -> Void)
        init(hoverCallback: @escaping ((CGPoint) -> Void)) {
            self.hoverCallback = hoverCallback
        }
        @objc func hovered(gesture:UIHoverGestureRecognizer) {
            let point = gesture.location(in: gesture.view)
            self.hoverCallback(point)
        }
    }

    public func makeCoordinator() -> HoverView.Coordinator {
        return Coordinator(hoverCallback:self.hoverCallback)
    }

    public func updateUIView(_ uiView: UIView,
                       context: UIViewRepresentableContext<HoverView>) {
    }
}
public struct PointInsideChecker: View {
    let rect: Rect
    @State var hoverLocation: CGPoint?
    public init(rect: Rect) {
        self.rect = rect
    }
    public var body: some View {
        ZStack {
            Rect.View(rect)
            if let hoverLocation = hoverLocation {
                let inside = Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(Float(hoverLocation.x), Float(hoverLocation.y)))
                let circleColor: Color = inside ? .red : .blue
                Circle().offset(hoverLocation).size(width: 5, height: 5).foregroundColor(circleColor)
            }
            HoverView { (point) in
                self.hoverLocation = point
            }
        }.padding(20)
        
    }
}




let rect = Rect(center: SIMD2<Float>(50,50), lengths: SIMD2<Float>(100,100), angle: 0)
PlaygroundPage.current.setLiveView(PointInsideChecker(rect: rect))
