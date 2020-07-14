//: [Previous](@previous)
import blitcurve
import SwiftUI
import PlaygroundSupport
let b = Box(a: SIMD2<Float>(11.93, 8.075), b: SIMD2<Float>(9.163, 11.93), c: SIMD2<Float>(10.84, 11.93))


PlaygroundPage.current.setLiveView(Box.View(b).scale(by: 10))
print(b.d)
//: [Next](@next)
