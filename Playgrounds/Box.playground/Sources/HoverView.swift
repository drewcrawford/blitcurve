import SwiftUI

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

