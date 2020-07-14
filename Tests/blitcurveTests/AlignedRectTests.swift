// AlignedRectTests.swift: AlignedRect tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve

class AlignedRectTests: XCTestCase {
    func testApartCorners() {
        let a = AlignedRect(min: .zero, max: .zero)
        let b = AlignedRect(min: .one, max: .one)
        XCTAssert(!a.isCornerWithinDistance(to: b, distance: 0.5))
        XCTAssert(a.isCornerWithinDistance(to: b, distance: sqrt(2)+0.01))
    }
    
    func testFarCorners() {
        let a = AlignedRect(min: .zero, max: .zero)
        let b = AlignedRect(min: .one, max: .one)
        XCTAssert(!a.isCornerWithinDistance(to: b, distance: 0.5))
        XCTAssert(a.isCornerWithinDistance(to: b, distance: sqrt(2)+0.01))
    }
    
    func testCenterPoint() {
        let a = AlignedRect(min: .one, max: SIMD2<Float>(2,2))
        XCTAssert(a.center == SIMD2<Float>(1.5,1.5))
    }
    
    static var allTests = [
        ("testApartCorners", testApartCorners),
        ("testFarCorners", testFarCorners),
        ("testCenterPoint", testCenterPoint),
    ]
}
