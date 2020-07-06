// Quicklook.swift: Xcode previews
// ©2020 DrewCrawfordApps LLC

#if os(macOS)
import AppKit
#elseif os(iOS)
import UIKit
#endif
import blitcurve_c

import CoreGraphics
private extension CGPoint {
    init(simd: SIMD2<Float>) {
        self.init(x: CGFloat(simd.x), y: CGFloat(simd.y))
    }
}
private extension CGRect {
    init(around: CGPoint, size: CGSize) {
        let origin = CGPoint(x: around.x - size.width / 2, y: around.x - size.height / 2)
        self.init(origin: origin, size: size)
    }
}
extension Cubic: CustomPlaygroundDisplayConvertible {
    public var playgroundDescription: Any {
        let alignedBox = AlignedBox(cubic: self, strategy: .fastest)
        let padding: CGFloat = 20
        let sizeForDrawing = CGSize(width: CGFloat(alignedBox.max.x) + padding, height: CGFloat(alignedBox.max.y) + padding)
        #if os(macOS)
        let image = NSImage(size: sizeForDrawing)
        image.lockFocus()
        let context = NSGraphicsContext.current!.cgContext
        #elseif os(iOS)
        UIGraphicsBeginImageContext(sizeForDrawing)
        let context = UIGraphicsGetCurrentContext()!
        #endif
        if #available(iOS 13.0, *) {
            context.setStrokeColor(CGColor(gray: 0, alpha: 1.0))
        } else {
            preconditionFailure("Not supported on this OS")
        }
        context.setLineWidth(1)
        //context.beginPath()
        context.move(to: CGPoint(simd: a))
        context.addCurve(to: CGPoint(simd: b), control1: CGPoint(simd: c), control2: CGPoint(simd: d))
        context.strokePath()
        NSString("a").draw(at: CGPoint(simd: a), withAttributes: nil)
        NSString("b").draw(at: CGPoint(simd: b), withAttributes: nil)
        NSString("c").draw(at: CGPoint(simd: c), withAttributes: nil)
        NSString("d").draw(at: CGPoint(simd: d), withAttributes: nil)
        #if os(macOS)
        image.unlockFocus()
        return image
        #elseif os(iOS)
        let image = UIGraphicsGetImageFromCurrentImageContext()!
        UIGraphicsEndImageContext()
        return image
        #endif
    }
    
}
