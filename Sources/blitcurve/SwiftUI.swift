//SwiftUI.swift: SwiftUI Extensions
// ©2020 DrewCrawfordApps LLC
import Foundation
import SwiftUI

@available(OSX 10.15.0, iOS 13.0.0, *)
private extension CGPoint {
    init(_ simd: SIMD2<Float>, scale: CGFloat) {
        self.init(x: CGFloat(simd.x) * scale, y: CGFloat(simd.y) * scale)
    }
}


@available(OSX 10.15.0, iOS 13.0.0, *)
fileprivate struct PointView: SwiftUI.View {
    @Environment(\.scale) var scale

    var body: some View {
        Text(label).padding(EdgeInsets(top: CGFloat(coordinate.y) * scale, leading: CGFloat(coordinate.x) * scale, bottom: 0, trailing: 0))
    }
    let label: String
    let coordinate: SIMD2<Float>
    init(_ label: String, coordinate: SIMD2<Float>) {
        self.label = label
        self.coordinate = coordinate
    }
}


//@available(OSX 10.15.0, iOS 13.0.0, *)
//
//private final class _AnyDrawableBox: _AnyDrawableBoxBase {
//
//}




@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic {
    public struct View: SwiftUI.View {
                
        let cubic: Cubic
        @Environment(\.scale) private var scale
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                PointView("a", coordinate: cubic.a)
                PointView("b", coordinate: cubic.b)
                PointView("c", coordinate: cubic.c)
                PointView("d", coordinate: cubic.d)
                Path { path in
                    path.move(to: CGPoint(cubic.a, scale: scale))
                    path.addCurve(to: CGPoint(cubic.b, scale: scale), control1: CGPoint(cubic.c, scale: scale), control2: CGPoint(cubic.d, scale: scale))
                }.stroke(lineWidth: 1)

            }
            
        }
        public init(_ cubic: Cubic) {
            self.cubic = cubic
        }
    }
}




@available(OSX 10.15.0, iOS 13.0.0, *)
public struct ScaledView<Content>: View where Content: View {
    let requestedScale: CGFloat
    let _body: () -> Content
    public var body: some View {
        _body().environment(\.scale, requestedScale)
    }
}


@available(OSX 10.15.0, iOS 13.0.0, *)
extension Box {
    public struct View: SwiftUI.View {
        private let box: Box
        @Environment(\.scale) private var scale
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                PointView("a", coordinate: box.a)
                PointView("b", coordinate: box.b)
                PointView("c", coordinate: box.c)
                PointView("d", coordinate: box.d)
                Path { path in
                    path.addLines([CGPoint(box.a, scale: scale),CGPoint(box.b, scale: scale),CGPoint(box.c, scale: scale),CGPoint(box.d, scale: scale),CGPoint(box.a, scale: scale)])
                }.stroke()

            }
        }
        public init(_ box: Box) {
            self.box = box
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
private struct ScaleKey: EnvironmentKey {
    static var defaultValue: CGFloat { 1 }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension EnvironmentValues {
    var scale: CGFloat {
        get {
            return self[ScaleKey]
        }
        set {
            self[ScaleKey] = newValue
        }
    }
}


///Variant of ZStack, appropriate for grouping blitcurve types for drawing into the same
///coordinate space
@available(OSX 10.15.0, iOS 13.0.0, *)
public struct BStack<Content>: View where Content: View {
    let viewBuilder: () -> Content
    public init(@ViewBuilder children: @escaping () -> Content) {
        self.viewBuilder = children
    }
    public var body: some View {
        ZStack(alignment: .topLeading) {
            viewBuilder()
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension BStack: Drawable { }
@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic.View: Drawable { }

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Box.View: Drawable { }

//this workflow is broken due to FB7872591
@available(OSX 10.15.0, iOS 13.0.0, *)
private struct Previews: PreviewProvider {
    static var previews: some View {
        /*@START_MENU_TOKEN@*/Text("Hello, World!")/*@END_MENU_TOKEN@*/
    }
}



