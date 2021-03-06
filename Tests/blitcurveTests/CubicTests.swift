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
        let _ = c.initialTangentAngle
        let _ = c.finalTangentAngle
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
        curve.normalize(approximateDistance: 0.001)
        XCTAssertNotEqual(curve.a, curve.c)
        XCTAssertNotEqual(curve.b,curve.d)
        
        var curve2 = Cubic(a: .zero, b: SIMD2<Float>(100,0), c: SIMD2<Float>(10,0), d: SIMD2<Float>(90,0))
        curve2.normalize(approximateDistance: 15)
        XCTAssertEqual(curve2.c,SIMD2<Float>(15,0))
        XCTAssertEqual(curve2.d, SIMD2<Float>(85,0), accuracy: 0.01)

    }
    
    func testConnectingTangents() {
        let railLine = Line(a: SIMD2<Float>(x: 510.37002139454205, y: 343.83395079162915), b: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134))
        let finalTangent = Line(a: SIMD2<Float>(x: 1712.387147298103, y: 298.4828489580134), b: SIMD2<Float>(x: 1808.2719716869988, y: 244.57458936905402)).tangent
        let initialTangent = Line(a: SIMD2<Float>(x: 376.872572406239, y: 106.38648819110057), b: SIMD2<Float>(x: 510.37002139454205, y: 343.83395079162915)).tangent
        let cube = Cubic(connecting: railLine, tangents: SIMD2<Float>(initialTangent, finalTangent), distances: SIMD2<Float>(1,1))
        XCTAssertEqual(cube.initialTangentAngle, initialTangent, accuracy: 0.1)
        XCTAssertEqual(cube.finalTangentAngle, finalTangent, accuracy: 0.1)
    }
    
    func testConnectingCubics() {
        let aRail = Cubic(a: SIMD2<Float>(2.6605175, 9.86255), b: SIMD2<Float>(0.21252254, 5.5065603), c: SIMD2<Float>(1.8522768, 8.424357), d: SIMD2<Float>(1.0362785, 6.9723616))
        let nextRail = Cubic(a: SIMD2<Float>(4.6373005, 3.1232715), b: SIMD2<Float>(7.0850296, 7.478789), c: SIMD2<Float>(5.4609714, 4.5889215), d: SIMD2<Float>(6.276881, 6.04076))
        let c = Cubic(connecting: aRail, to: nextRail, tangentRule: .copied)
        
        XCTAssertEqual(aRail.b, c.a)
        XCTAssertEqual(nextRail.a, c.b)
        XCTAssertEqual(c.initialTangentAngle, aRail.finalTangentAngle)
        //final tangent is reversed nextRail.initialTangent
        XCTAssertEqual(c.finalTangentAngle, nextRail.initialTangentAngle, accuracy: 0.1)
    }
    
    func testConnectingLines() {
        let a = Line(a: .zero, b: SIMD2<Float>(10,10))
        let b = Line(a: SIMD2<Float>(20,20), b: SIMD2<Float>(30,30))
        let c = Cubic(connecting: a, to: b)
        XCTAssertEqual(c.a, a.b)
        XCTAssertEqual(c.b, b.a)
        XCTAssertEqual(c.initialTangentAngle, a.tangent)
        XCTAssertEqual(c.finalTangentAngle, b.tangent)
        XCTAssertEqual(c.initialTangentMagnitude, c.finalTangentMagnitude)
        XCTAssertEqual(c.initialTangentMagnitude, c.finalTangentMagnitude)
        XCTAssertEqual(c.initialTangentMagnitude, 7.07, accuracy: 0.01)
    }
    
    func testConnectingCubicToPoint() {
        let c = Cubic(a: .zero, b: SIMD2<Float>(10,10), c: SIMD2<Float>(0,3), d: SIMD2<Float>(0,7))
        let c2 = Cubic(connecting: c, to: SIMD2<Float>(10,15), finalTangent: 0, initialTangentRule: .copied)
        XCTAssertEqual(c2.a, c.b)
        XCTAssertEqual(c2.b,SIMD2<Float>(10,15))
        XCTAssertEqual(c2.initialTangentAngle, c.finalTangentAngle)
        XCTAssertEqual(c2.initialTangentMagnitude, c.finalTangentMagnitude)
        XCTAssertEqual(c2.finalTangentAngle,0)
        XCTAssertEqual(c2.finalTangentMagnitude,2.5)
    }
    
    
    func testLength() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        XCTAssertEqual(curve.length, 272.87, accuracy: 20)
    }
    
    func testAlignedRect() {
        let curve = Cubic(a: SIMD2<Float>(x: 120, y: 60), b: SIMD2<Float>(x: 220, y: 40), c: SIMD2<Float>(x: 35, y: 200), d: SIMD2<Float>(x: 220, y: 260))
        let box = AlignedRect(cubic: curve, strategy: .fastest)
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
        
        let rightSplit = curve.rightSplit(t: 0.5)
        XCTAssertEqual(rightSplit.a, curves.right.a)
        XCTAssertEqual(rightSplit.b, curves.right.b)
        XCTAssertEqual(rightSplit.c, curves.right.c)
        XCTAssertEqual(rightSplit.d, curves.right.d)
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
        
        //due to mt2-99, we want to be allowed to parameterize slightly out of range
        XCTAssertEqual(cubic.parameterization(arclength: cubic.length+0.5, threshold: 0.000000001), 1)
    }
    
    func testTangent() {
        let a = SIMD2<Float>(x: 192, y: 341)
        let nearA = SIMD2<Float>(x: 193.83, y: 344.26)
        var c = Cubic(a: a, b: SIMD2<Float>(x: 375, y: 667), c:nearA , d:nearA)
        XCTAssert(c.isNearlyLinear(accuracy: 0.01) != 0)
        c.normalize(approximateDistance: 6)
        XCTAssertEqual(c.tangentAt(t: 0.5), 1.0592812)
        
        //here are some tangents we've found to be "bad" with other approaches
        let t1 = c.tangentAt(t: 0.10)
        let t2 = c.tangentAt(t: 0.13)
        XCTAssertEqual(t1, t2, accuracy: 0.1)
        
        let b = SIMD2<Float>(x: 375, y: 667)
        let nearB = SIMD2<Float>(x: 376, y: 668)
        var d = Cubic(a: SIMD2<Float>(x: 192, y: 341), b: b, c: nearB, d: nearB)
        d.normalize(approximateDistance: 5)
        let d1 = d.tangentAt(t: 0.94)
        let d2 = d.tangentAt(t: 0.95)
        XCTAssertEqual(d1, d2, accuracy: 0.1)
        
    }
    
    func testCurvatureError() {
        let m = NormalizationDistanceForCubicCurvature(euclidianDistance: 20, straightAngle: 2 * .pi / 360, curvatureError: 0.02)
        XCTAssertEqual(m,2.87,accuracy: 0.01)
    }
    
    func testAlignedForCurvature() {
        let c = Cubic(a: .zero, b: SIMD2<Float>(20,0), c: SIMD2<Float>(5,0), d: SIMD2<Float>(17,0))
        XCTAssert(c.isNormalizedForCurvature(straightAngle: 2 * .pi/360, curvatureError: 0.02))
        let a = AlignedCubic(cubic: c)
        XCTAssert(a.isNormalizedForCurvature(straightAngle: 2 * .pi/360, curvatureError: 0.02))
        
        let badCubic = Cubic(a: .zero, b: SIMD2<Float>(20,0), c: SIMD2<Float>(2,0), d: SIMD2<Float>(17,0))
        XCTAssert(!badCubic.isNormalizedForCurvature(straightAngle: 2 * .pi/360, curvatureError: 0.02))
        let badA = AlignedCubic(cubic: badCubic)
        XCTAssert(!badA.isNormalizedForCurvature(straightAngle: 2 * .pi/360, curvatureError: 0.02))
    }
    
    func testDescription() {
        let c = Cubic(a: BCFloat2(0,1), b: BCFloat2(2,3), c: BCFloat2(4,5), d: BCFloat2(6,7))
        let canonicalForm = c.description
        
        //string should be the same as the constructor call below
        XCTAssertEqual(canonicalForm, "Cubic(a: BCFloat2(x: 0.0, y: 1.0), b: BCFloat2(x: 2.0, y: 3.0), c: BCFloat2(x: 4.0, y: 5.0), d: BCFloat2(x: 6.0, y: 7.0))")
        let c2 =                       Cubic(a: BCFloat2(x: 0.0, y: 1.0), b: BCFloat2(x: 2.0, y: 3.0), c: BCFloat2(x: 4.0, y: 5.0), d: BCFloat2(x: 6.0, y: 7.0))
        XCTAssertEqual(c, c2)
    }
    
}
