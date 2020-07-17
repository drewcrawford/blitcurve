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
public struct PointView: SwiftUI.View {
    @Environment(\.scale) var scale

    public var body: some View {
        ZStack(alignment:.topLeading) {
            Circle().fill().frame(width: 1, height: 1, alignment: .center)
            Text(label).padding(EdgeInsets(top: 2, leading: 2, bottom: 0, trailing: 0))
        }.padding(EdgeInsets(top: CGFloat(coordinate.y) * scale, leading: CGFloat(coordinate.x) * scale, bottom: 0, trailing: 0))
    }
    let label: String
    let coordinate: SIMD2<Float>
    public init(_ label: String, coordinate: SIMD2<Float>) {
        self.label = label
        self.coordinate = coordinate
    }
}

///BScene reads this in order to crop the scene to the minimum point
fileprivate struct MinCoordinatePreferenceKey: PreferenceKey {
    static var defaultValue: SIMD2<Float> { return .zero }
    static func reduce(value: inout SIMD2<Float>, nextValue: () -> SIMD2<Float>) {
        value = reduceMin(a: value, b: nextValue())
    }
}

///This protocol provides various useful features for types that have points
private protocol HasPoints {
    var points: [SIMD2<Float>] { get }
}
private func reduceMin(a: SIMD2<Float>, b: SIMD2<Float>) -> SIMD2<Float> {
    var a = a
    if b.x < a.x {
        a.x = b.x
    }
    if b.y < a.y {
        a.y = b.y
    }
    return a
}
@available(OSX 10.15.0, iOS 13.0.0, *)
extension HasPoints {
    var pointViews: [PointView] {
        let labels = ["a","b","c","d"]
        var views: [PointView] = []
        for (p,point) in points.enumerated() {
            views.append(PointView(labels[p], coordinate: point))
        }
        return views
    }
    ///Returns the minimum X/Y coordinate for this type.  If the receiver contains no points, this will return `.infinity`
    ///- note: This is used to crop the view hierarchy to an interesting region
    var minPoint: SIMD2<Float> {
        points.reduce(SIMD2<Float>(.infinity,.infinity), reduceMin)
    }
}




@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic {
    public struct View: SwiftUI.View {
                
        let cubic: Cubic
        @Environment(\.scale) private var scale
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                ForEach(0..<pointViews.count) {
                    pointViews[$0]
                }
                Path { path in
                    path.move(to: CGPoint(cubic.a, scale: scale))
                    path.addCurve(to: CGPoint(cubic.b, scale: scale), control1: CGPoint(cubic.c, scale: scale), control2: CGPoint(cubic.d, scale: scale))
                }.stroke(lineWidth: 1).preference(key: MinCoordinatePreferenceKey, value: minPoint)

            }
            
        }
        public init(_ cubic: Cubic) {
            self.cubic = cubic
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic.View: HasPoints {
    var points: [SIMD2<Float>] {
        [cubic.a,cubic.b,cubic.d,cubic.d]
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
extension Rect {
    public struct View: SwiftUI.View {
        fileprivate let box: Rect
        @Environment(\.scale) private var scale
        public var body: some SwiftUI.View {
            let a = box.points4.a_b.lowHalf
            let b = box.points4.a_b.highHalf
            let c = box.points4.c_d.lowHalf
            let d = box.points4.c_d.highHalf
            return ZStack(alignment: .topLeading) {
                ForEach(0..<pointViews.count) {
                    pointViews[$0]
                }
                Path { path in
                    path.addLines([CGPoint(a, scale: scale),CGPoint(b, scale: scale),CGPoint(c, scale: scale),CGPoint(d, scale: scale),CGPoint(a, scale: scale)])
                }.stroke().preference(key: MinCoordinatePreferenceKey.self, value: minPoint)

            }
        }
        public init(_ box: Rect) {
            self.box = box
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Rect.View: HasPoints {
    var points: [SIMD2<Float>] {
        let points = box.points4
        return [points.a_b.lowHalf,points.a_b.highHalf,points.c_d.lowHalf,points.c_d.highHalf]
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
    @State var offset: SIMD2<Float> = .zero
    @Environment(\.scale) var scale: CGFloat
    public init(@ViewBuilder children: @escaping () -> Content) {
        self.viewBuilder = children
    }
    public var body: some View {
        ZStack(alignment: .topLeading) {
            viewBuilder()
        }.onPreferenceChange(MinCoordinatePreferenceKey) { value in
            self.offset = value
        }.offset(x: -CGPoint(offset, scale: scale).x, y: -CGPoint(offset, scale: scale).y)
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension BStack: Drawable { }
@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic.View: Drawable { }

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Rect.View: Drawable { }
@available(OSX 10.15.0, iOS 13.0.0, *)
extension PointView: Drawable { }

//this workflow is broken due to FB7872591
@available(OSX 10.15.0, iOS 13.0.0, *)
private struct Previews: PreviewProvider {
    static var previews: some View {
        /*@START_MENU_TOKEN@*/Text("Hello, World!")/*@END_MENU_TOKEN@*/
    }
}



