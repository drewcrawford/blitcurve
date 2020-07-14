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
    
    func testAngle() {
        let box = Box(center: SIMD2<Float>(50,50),angle: 0, lengths: SIMD2<Float>(25,25))
        
    }
    func testD() {
        let b = Box(a: SIMD2<Float>(11.93, 8.075), b: SIMD2<Float>(9.163, 11.93), c: SIMD2<Float>(10.84, 11.93))
        XCTAssertEqual(b.d, SIMD2<Float>(13.607001, 8.075001))
        
    }
    static var allTests = [
        ("testPointInside",testPointInside),
        ("testD",testD),
        ("testAngle",testAngle),
    ]
}
