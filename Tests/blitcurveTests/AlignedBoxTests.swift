// cubictests.swift: AlignedBox tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve

class AlignedBoxTests: XCTestCase {
    func testApartCorners() {
        let a = AlignedBox(min: .zero, max: .zero)
        let b = AlignedBox(min: .one, max: .one)
        XCTAssert(!a.isCornerWithinDistance(to: b, distance: 0.5))
        XCTAssert(a.isCornerWithinDistance(to: b, distance: sqrt(2)+0.01))
    }
    
    func testFarCorners() {
        let a = AlignedBox(min: .zero, max: .zero)
        let b = AlignedBox(min: .one, max: .one)
        XCTAssert(!a.isCornerWithinDistance(to: b, distance: 0.5))
        XCTAssert(a.isCornerWithinDistance(to: b, distance: sqrt(2)+0.01))
    }
}
