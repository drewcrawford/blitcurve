// Line2Tests.swift: Line2 tests
// ©2020 DrewCrawfordApps LLC
import XCTest
 import blitcurve

final class Line2Tests: XCTestCase {
    func testLine() {
        let l = Line2(point: .zero, angle: SIMD2<Float>(.pi/2,.pi/4), distance: SIMD2<Float>(12,13))
        XCTAssert(l.a == .zero)
        XCTAssert(l.b.lowHalf =~= SIMD2<Float>(0,12))
        
        let side = sqrtf(13*13/2)
        XCTAssert(l.b.highHalf =~= BCFloat2(side, side));
    }

    static var allTests = [
        ("testLine", testLine),
    ]
}
