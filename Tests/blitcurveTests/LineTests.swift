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

    static var allTests = [
        ("testLine", testLine),
        ("testSlope", testSlope),
    ]
}
