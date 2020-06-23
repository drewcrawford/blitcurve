// cubictests.swift: Cubic tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import simd
import blitcurve
import blitcurve_c


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
    
    func testLength() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        XCTAssertEqual(curve.length, 272.87, accuracy: 20)
        
    }
    
    func testAlignedBox() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        let box = AlignedBox(cubic: curve, strategy: .fastest)
        XCTAssertEqual(box.min, SIMD2<Float>(35,40))
        XCTAssertEqual(box.max, SIMD2<Float>(220,260))
    }
    
    func testSplit() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        let curves = curve.split(t: 0.5)
        
        //endpoints
        XCTAssertEqual(curves.left.a, curve.a)
        XCTAssertEqual(curves.right.b, curve.b)
        
        XCTAssertEqual(curves.left.b, SIMD2<Float>(138.125, 185.0))
        XCTAssertEqual(curves.left.c, SIMD2<Float>(77.5, 130.0))
        XCTAssertEqual(curves.left.d, SIMD2<Float>(102.5, 180.0))
        
        XCTAssertEqual(curves.right.a, SIMD2<Float>(138.125, 185.0))
        XCTAssertEqual(curves.right.c, SIMD2<Float>(173.75, 190.0))
        XCTAssertEqual(curves.right.d, SIMD2<Float>(220.0, 150.0))
        
        let leftSplit = curve.leftSplit(t: 0.5)
        XCTAssertEqual(leftSplit.a, curves.left.a)
        XCTAssertEqual(leftSplit.b, curves.left.b)
        XCTAssertEqual(leftSplit.c, curves.left.c)
        XCTAssertEqual(leftSplit.d, curves.left.d)
    }
    #if DEBUG
    #else
    //only do this test in release mode
    //this test is slow
    func testLengthPerformance() {
        //generate 100 curves
        var curves: [Cubic] = []
        for _ in 0..<100 {
            let a = SIMD2<Float>(x: Float.random(in: 0..<100), y: Float.random(in: 0..<100))
            let b = SIMD2<Float>(x: Float.random(in: 0..<100), y: Float.random(in: 0..<100))
            let c = SIMD2<Float>(x: Float.random(in: 0..<100), y: Float.random(in: 0..<100))
            let d = SIMD2<Float>(x: Float.random(in: 0..<100), y: Float.random(in: 0..<100))
            let cubic = Cubic(a: a, b: b, c: c, d: d)
            curves.append(cubic)
        }
        self.measure {

            var r: Float = 0
            for _ in 0..<100000 {
                for c in curves {
                    r += c.length
                }
            }
            print("a",r)
        }
    }
    #endif
    
    func testParametrization() {
        let cubic = Cubic(a: .zero, b: SIMD2<Float>(x: 100, y: 0), c: SIMD2<Float>(x: 66.6667, y: 0), d: SIMD2<Float>(x: 100, y: 0))
        let parameterization = cubic.parameterization(arclength: 50, threshold: 0.01)
        XCTAssertEqual(parameterization, 0.289, accuracy: 0.01)
    }
    
    static var allTests: [(String,(CubicTests) -> () -> ())] = {
        
        var tests = [
        ("testTangents", testTangents),
        ("testEvaluate", testEvaluate),
        ("testNormalize", testEvaluate),
        ("testLength",testLength),
        ("testSplit",testSplit),
        ("testParameterization",testParametrization)

        ]
        
        
        #if DEBUG
        #else
        tests.append(("testLengthPerformance",testLengthPerformance))

        #endif
        return tests
    }()
}
