// cubictests.swift: AlignedBox tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve

class BoxTests: XCTestCase {
    func testPointInside() {
        let box = Box(a: SIMD2<Float>(0,0), b: SIMD2<Float>(0,100),c: SIMD2<Float>(100,100))
        XCTAssert(box.isPointOnOrInside(SIMD2<Float>(50,50)))
        XCTAssert(box.isPointOnOrInside(SIMD2<Float>(0,0)))
        XCTAssert(!box.isPointOnOrInside(SIMD2<Float>(101,90)))
        XCTAssert(!box.isPointOnOrInside(SIMD2<Float>(101,90)))
        XCTAssert(!box.isPointOnOrInside(SIMD2<Float>(-1,90)))
    }
    
    static var allTests = [
        ("testPointInside",testPointInside),
    ]
}
