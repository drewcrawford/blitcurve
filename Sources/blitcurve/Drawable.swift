//Drawable.swift: SwiftUI generic types
// ©2020 DrewCrawfordApps LLC

import SwiftUI

@available(OSX 10.15.0, iOS 13.0.0, *)
public protocol Drawable: View {
}

@available(OSX 10.15.0, iOS 13.0.0, *)
extension Drawable {
    public func scale(by scale: CGFloat) -> ScaledView<Self> {
        ScaledView(requestedScale: scale, _body: { self })
    }
}

//@available(OSX 10.15.0, iOS 13.0.0, *)
//public protocol _AnyDrawableBoxBase {
//    func scale(by scale: CGFloat) -> AnyDrawable
//    func applying(_ context: SceneContext) -> AnyDrawable
//    var body: AnyView { get }
//}
//@available(OSX 10.15.0, iOS 13.0.0, *)
//struct _AnyDrawableBox<D: Drawable>: _AnyDrawableBoxBase {
//    var base: D
//    func scale(by scale: CGFloat) -> _AnyDrawableBox<D> {
//        base.scale(by: <#T##CGFloat#>)
//    }
//}
//@available(OSX 10.15.0, iOS 13.0.0, *)
//public struct AnyDrawable: Drawable {
//    let base: _AnyDrawable
//    public func scale(by scale: CGFloat) -> AnyDrawable {
//        base.scale(by: scale)
//    }
//    public func applying(_ context: SceneContext) -> AnyDrawable {
//        base.applying(context)
//    }
//    public var body: some View {
//        return base.view
//    }
//    public init<D: Drawable>(_ drawable: D) {
//        self.base =
//    }
//}
