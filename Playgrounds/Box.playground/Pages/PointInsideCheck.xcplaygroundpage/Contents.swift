
import SwiftUI
import PlaygroundSupport
import blitcurve

//note: This cannot be extracted due to FB7859836 I believe
public struct PointInsideChecker: View {
    let box: Box
    @State var hoverLocation: CGPoint?
    public init(box: Box) {
        self.box = box
    }
    public var body: some View {
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




let box = Box(a: SIMD2<Float>(0,0), b: SIMD2<Float>(0,100),c: SIMD2<Float>(100,100))
PlaygroundPage.current.setLiveView(PointInsideChecker(box: box))
