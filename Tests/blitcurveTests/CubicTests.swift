// cubictests.swift: Cubic tests
// ©2020 DrewCrawfordApps LLC

import XCTest
@testable import blitcurve

final class CubicTests: XCTestCase {
    func testTangents() {
        //mostly want to make sure we can access these symbols
        let c = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(1,1), c: SIMD2<Float>(2,2), d: SIMD2<Float>(3,3))
        let _ = c.initialTangent
        let _ = c.finalTangent
    }
    
    static var allTests = [
        ("testTangents", testTangents),
    ]
}
