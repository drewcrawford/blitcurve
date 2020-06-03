// linetests.swift: Line tests
// ©2020 DrewCrawfordApps LLC
import XCTest
@testable import blitcurve

final class LineTests: XCTestCase {
    func testLine() {
        let l = Line(point:SIMD2<Float>(0,0), angle: .pi / 2, distance: 12)
        XCTAssert(l.a == .zero)
        XCTAssert(l.b ~= SIMD2<Float>(0,12))
        
        let l2 = Line(point:SIMD2<Float>(0,0), angle: .pi / 4, distance: 12)
        XCTAssert(l2.a == .zero)
        let side = sqrtf(12*12/2)
        XCTAssert(l2.b ~= BCFloat2(side, side));
    }
    
    func testSlope() {
        let l2 = Line(point:SIMD2<Float>(0,0), angle: .pi / 4, distance: 12)
        XCTAssert(l2.slope ~= 1)
    }
    
    func testYIntercept() {
        let l2 = Line(point:SIMD2<Float>(0,0), angle: .pi / 4, distance: 12)
        XCTAssert(l2.yIntercept ~= 0)
    }
    
    func testLength() {
        let l2 = Line(a: .zero, b: SIMD2<Float>(3,3))
        XCTAssert(l2.distance ~= 4.24264)
        
        let l0 = Line(a: .zero, b:.zero)
        XCTAssert(l0.distance == 0)
    }
    
    func testTangent() {
        let l2 = Line(a: .zero, b: SIMD2<Float>(3,3))
        XCTAssert(l2.tangent ~= .pi / 4)
    }
    
    func testEvaluate() {
        let l = Line(a: .zero, b: SIMD2<Float>(100,100))
        XCTAssertEqual(l.evaluate(t: 0),  l.a)
        XCTAssertEqual(l.evaluate(t: 1),  l.b)
        XCTAssertEqual(l.evaluate(t: 0.5),  SIMD2<Float>(50,50))
        
        //try a line with 0-length
        let l0 = Line(a: .zero, b:.zero)
        XCTAssertEqual(l0.evaluate(t: 0), .zero)
        XCTAssertEqual(l0.evaluate(t: 1), .zero)
        XCTAssertEqual(l0.evaluate(t: 0.5), .zero)
    }

    static var allTests = [
        ("testLine", testLine),
        ("testSlope", testSlope),
        ("testLength",testLength),
        ("testTangent",testTangent),
        ("testEvaluate",testEvaluate)
    ]
}
