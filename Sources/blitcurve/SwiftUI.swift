//SwiftUI.swift: SwiftUI Extensions
// ©2020 DrewCrawfordApps LLC
import Foundation
import SwiftUI

@available(OSX 10.15.0, iOS 13.0.0, *)
private extension CGPoint {
    init(_ simd: SIMD2<Float>, context: SceneContext) {
        self.init(x: CGFloat(simd.x) * context.scale, y: CGFloat(simd.y) * context.scale)
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
private struct SceneContext {
    var scale: CGFloat
    
    ///Creates a default context
    init() {
        scale = 1
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
fileprivate struct PointView: SwiftUI.View {
    var body: some View {
        Text(label).padding(EdgeInsets(top: CGFloat(coordinate.y) * context.scale, leading: CGFloat(coordinate.x) * context.scale, bottom: 0, trailing: 0))
    }
    let label: String
    let coordinate: SIMD2<Float>
    let context: SceneContext
    init(_ label: String, coordinate: SIMD2<Float>, context: SceneContext) {
        self.label = label
        self.coordinate = coordinate
        self.context = context
    }
}
public protocol Drawable: View {
    func scale(by: CGFloat) -> Self
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic {
    public struct View: SwiftUI.View {
                
        private let context: SceneContext
        let cubic: Cubic
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                PointView("a", coordinate: cubic.a, context: context)
                PointView("b", coordinate: cubic.b, context: context)
                PointView("c", coordinate: cubic.c, context: context)
                PointView("d", coordinate: cubic.d, context: context)
                Path { path in
                    path.move(to: CGPoint(cubic.a, context: context))
                    path.addCurve(to: CGPoint(cubic.b, context: context), control1: CGPoint(cubic.c, context: context), control2: CGPoint(cubic.d, context: context))
                }.stroke(lineWidth: 1)

            }
            
        }
        public init(_ cubic: Cubic) {
            self.cubic = cubic
            self.context = SceneContext()
        }
        fileprivate init(_ cubic: Cubic, context: SceneContext) {
            self.cubic = cubic
            self.context = context
        }
    }
}



@available(iOS 13.0.0, *)
extension Cubic.View: Drawable {
    public func scale(by scale: CGFloat) -> Self {
        var context = self.context
        context.scale = scale
        return Cubic.View(cubic, context: context)
    }
}

@available(iOS 13.0.0, *)
private extension View {
    ///Returns the view framed for the given points
    func enframe(points: [CGPoint]) -> some View {
        frame(minWidth: points.map(\.x).max(),  minHeight: points.map(\.y).max(), alignment: .topLeading)
    }
}

@available(iOS 13.0.0, *)
extension Box {
    public struct View: SwiftUI.View {
        private let box: Box
        private let context: SceneContext
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                PointView("a", coordinate: box.a, context: context)
                PointView("b", coordinate: box.b, context: context)
                PointView("c", coordinate: box.c, context: context)
                PointView("d", coordinate: box.d, context: context)
                Path { path in
                    path.addLines([CGPoint(box.a, context: context),CGPoint(box.b, context: context),CGPoint(box.c, context: context),CGPoint(box.d, context: context),CGPoint(box.a, context: context)])
                }.scale(context.scale, anchor: .topLeading).stroke(lineWidth: 1)

            }.enframe(points: [CGPoint(box.a, context: context),CGPoint(box.b, context: context),CGPoint(box.c, context: context),CGPoint(box.d, context: context)])
        }
        public init(_ box: Box) {
            self.box = box
            self.context = SceneContext()
        }
    }
}


//this workflow is broken due to FB7872591
@available(OSX 10.15.0, iOS 13.0.0, *)
private struct Previews: PreviewProvider {
    static var previews: some View {
        /*@START_MENU_TOKEN@*/Text("Hello, World!")/*@END_MENU_TOKEN@*/
    }
}



