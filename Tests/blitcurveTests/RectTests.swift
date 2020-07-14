// cubictests.swift: AlignedBox tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve

class BoxTests: XCTestCase {
    func testPointInside() {
        let rect = Rect(center: SIMD2<Float>(25,25), lengths: SIMD2<Float>(50,50), angle: 0)
        XCTAssert(Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(50,50)))
        XCTAssert(Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(0,0)))
        XCTAssert(!Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(101,90)))
        XCTAssert(!Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(101,90)))
        XCTAssert(!Rect.isPointOnOrInside(points: rect.points3, point: SIMD2<Float>(-1,90)))
    }
    

    static var allTests = [
        ("testPointInside",testPointInside),
    ]
}
