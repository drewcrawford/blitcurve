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
    
    func testEvaluate() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        let shouldBeA = curve.evaluate(t: 0)
        XCTAssertEqual(shouldBeA.x, curve.a.x)
        XCTAssertEqual(shouldBeA.y, curve.a.y)
        
        let shouldBeB = curve.evaluate(t: 1)
        XCTAssertEqual(shouldBeB.x, curve.b.x)
        XCTAssertEqual(shouldBeB.y, curve.b.y)
    }
    
    static var allTests = [
        ("testTangents", testTangents),
        ("testEvaluate", testEvaluate),
    ]
}
