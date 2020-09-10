//AlignedCubicTests.swift: Aligned cubic tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve_c

final class AlignedCubicTests: XCTestCase {
    func testMake() {
        let c = Cubic(a: SIMD2<Float>(120,160), b: SIMD2<Float>(220,40), c: SIMD2<Float>(35,200), d: SIMD2<Float>(220,260))
        let alignedCubic = AlignedCubic(cubic: c)
        XCTAssertEqual(alignedCubic.b_x, 156,accuracy:1)
        XCTAssertLessThan(simd_distance(alignedCubic.c, SIMD2<Float>(-85,-40)), 2)
        XCTAssertLessThan(simd_distance(alignedCubic.d, SIMD2<Float>(SIMD2<Float>(-12,140))), 2)
    }
    static var allTests = [
        ("testMake", testMake),
    ]
}
