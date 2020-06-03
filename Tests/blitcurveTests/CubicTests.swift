// cubictests.swift: Cubic tests
// ©2020 DrewCrawfordApps LLC

import XCTest
@testable import blitcurve

final class CubicTests: XCTestCase {
    func testTangents() {
        //mostly want to make sure we can access these symbols
        let c = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(1,1), c: SIMD2<Float>(2,2), d: SIMD2<Float>(3,3))
        let _ = c.initialTangentLine
        let _ = c.finalTangentLine
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
    
    func testNormalize() {
        var curve = Cubic(a: .zero, b: SIMD2<Float>(x: 100, y: 100), c: .zero, d: SIMD2<Float>(x: 100, y: 100))
        curve.normalize()
        XCTAssert(curve.a != curve.c)
        XCTAssert(curve.b != curve.d)
    }
    
    func testConnectingTangents() {
        let railLine = Line(a: SIMD2<Float>(x: 510.37002139454205, y: 343.83395079162915), b: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134))
        //let finalTangent = Line(a: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134), b: SIMD2<Float>(x: 1808.2719716869988, y: 244.57458936905402)).tangent
        let finalTangent = Line(a:SIMD2<Float>(x: 1808.2719716869988, y: 244.57458936905402), b: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134) ).tangent
        let initialTangent = Line(a: SIMD2<Float>(x: 376.872572406239, y: 106.38648819110057), b: SIMD2<Float>(x: 510.37002139454205, y: 343.83395079162915)).tangent
        let cube = Cubic(connecting: railLine, initialTangent: initialTangent, finalTangent: finalTangent)
        XCTAssertEqual(cube.initialTangent, initialTangent, accuracy: 0.1)
        XCTAssertEqual(cube.finalTangent, finalTangent, accuracy: 0.1)
    }
    
    func testConnectingCubics() {
        let railLine = Line(a: SIMD2<Float>(x: 510.37002139454205, y: 343.83395079162915), b: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134))
        
    }
    
    static var allTests = [
        ("testTangents", testTangents),
        ("testEvaluate", testEvaluate),
        ("testNormalize", testEvaluate),
    ]
}
