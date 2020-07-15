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
    func testPoints() {
        //we expect '0' to mean "pointing to the right" here.
        let rect = Rect(center: .zero, lengths: SIMD2<Float>(5,10), angle: 0)
        let points = rect.points4
        XCTAssertEqual(points.a_b.lowHalf, SIMD2<Float>(5,-2.5)) //a
        XCTAssertEqual(points.a_b.highHalf, SIMD2<Float>(5,2.5)) //b
        XCTAssertEqual(points.c_d.lowHalf, SIMD2<Float>(-5,2.5)) //c
        XCTAssertEqual(points.c_d.highHalf, SIMD2<Float>(-5,-2.5)) //d
        
        let points3 = rect.points3
        XCTAssertEqual(points3.a_b.lowHalf, SIMD2<Float>(5,-2.5)) //a
        XCTAssertEqual(points3.a_b.highHalf, SIMD2<Float>(5,2.5)) //b
        XCTAssertEqual(points3.c, SIMD2<Float>(-5,2.5)) //c

    }
    

    static var allTests = [
        ("testPointInside",testPointInside),
        ("testPoints",testPoints),

    ]
}
