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

        }
        //setting padding does not inherently change the frame of the view.
        //the frame here is frequently some viewport size, so if we set a top/leading padding that is well otuside of it,
        //the Text may try to crop, leading to mysterously missing points.
        //Setting the bottom /trailing to the equivalent negative value will allow us to extend outside the frame here
        .padding(EdgeInsets(top: CGFloat(coordinate.y) * scale, leading: CGFloat(coordinate.x) * scale, bottom: -CGFloat(coordinate.y) * scale, trailing: -CGFloat(coordinate.x) * scale))
        .background(GeometryReader { proxy in
            Color.clear //FBFB8061577
            .anchorPreference(key: MaxPointPreferenceKey.self, value: .bottomTrailing) { anchor in
                proxy[anchor]
            }
        })
        
    }
    let label: String
    let coordinate: SIMD2<Float>
    public init(_ label: String, coordinate: SIMD2<Float>) {
        self.label = label
        self.coordinate = coordinate
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
fileprivate struct MaxPointPreferenceKey: PreferenceKey {
    static var defaultValue: CGPoint {.zero}
    static func reduce(value: inout CGPoint, nextValue: () -> CGPoint) {
        let n = nextValue()
        if value.x < n.x {
            value.x = n.x
        }
        if value.y < n.y {
            value.y = n.y
        }
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
        @State private var maxCoordinate: CGPoint? = nil

        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                ForEach(0..<pointViews.count) {
                    pointViews[$0]
                }
                Path { path in
                    path.move(to: CGPoint(cubic.a, scale: scale))
                    path.addCurve(to: CGPoint(cubic.b, scale: scale), control1: CGPoint(cubic.c, scale: scale), control2: CGPoint(cubic.d, scale: scale))
                }.stroke(lineWidth: 1).preference(key: MinCoordinatePreferenceKey.self, value: minPoint)

            }
            //.border(Color.red, width: 2) //debugging purposes
            .frame(width: maxCoordinate?.x, height: maxCoordinate?.y, alignment: .topLeading)
            .onPreferenceChange(MaxPointPreferenceKey.self) { maxCoordinate = $0}
            
        }
        public init(_ cubic: Cubic) {
            self.cubic = cubic
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Cubic.View: HasPoints {
    var points: [SIMD2<Float>] {
        [cubic.a,cubic.b,cubic.c,cubic.d]
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
        @State private var preferredSize: CGSize? = nil
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
            .onPreferenceChange(MaxPointPreferenceKey.self) { value in
                self.preferredSize = CGSize(width: value.x, height: value.y)
            }
            //.border(Color.red, width: /*@START_MENU_TOKEN@*/1/*@END_MENU_TOKEN@*/)
            .frame(width: preferredSize?.width, height: preferredSize?.height, alignment: .topLeading)
            
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
extension Line {
    public struct View: SwiftUI.View {
        fileprivate let line: Line
        @Environment(\.scale) private var scale
        @State private var preferredSize: CGSize? = nil
        public var body: some SwiftUI.View {
            ZStack(alignment: .topLeading) {
                ForEach(0..<pointViews.count) {
                    pointViews[$0]
                }
                Path { path in
                    path.addLines([CGPoint(line.a, scale: scale),CGPoint(line.b, scale: scale)])
                }.stroke()
                .preference(key: MinCoordinatePreferenceKey.self, value: minPoint)
            }
            .onPreferenceChange(MaxPointPreferenceKey.self) { value in
                self.preferredSize = CGSize(width: value.x, height: value.y)
            }
            //.border(Color.red, width: /*@START_MENU_TOKEN@*/1/*@END_MENU_TOKEN@*/)
            .frame(width: preferredSize?.width, height: preferredSize?.height, alignment: .topLeading)
            
        }
        public init(_ line: Line) {
            self.line = line
        }
    }
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Line.View: HasPoints {
    var points: [SIMD2<Float>] {
        return [line.a,line.b]
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
    var viewBuilder: () -> Content
    @State var offset: SIMD2<Float> = .zero
    @State var maxPoint: CGPoint? = nil
    @Environment(\.scale) var scale: CGFloat
    public init(@ViewBuilder children: @escaping () -> Content) {
        self.viewBuilder = children
    }
    public var body: some View {
        ZStack(alignment: .topLeading) {
            viewBuilder()
        }
        .onPreferenceChange(MinCoordinatePreferenceKey.self) { value in
            self.offset = value
        }
        .onPreferenceChange(MaxPointPreferenceKey.self) { maxPoint = $0 }
        .offset(x: -CGPoint(offset, scale: scale).x, y: -CGPoint(offset, scale: scale).y)
        //.frame(width: maxPoint?.x, height: maxPoint?.y, alignment: .topLeading)
        //note that setting a frame has no effect in playgrounds: FB8061500

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



