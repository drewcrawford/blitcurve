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
    
    func testPointsWithRotation() {
        //This test case also checks the rotation, by providing a non-zero angle
        let r = Rect(center: SIMD2<Float>(58.491684, 109.1976), lengths: SIMD2<Float>(1.675, 3.85), angle: -2.0864184)
        let points = r.points4
        
        XCTAssertEqual(points.a_b, SIMD4<Float>(56.8139, 107.93583, 58.271126, 107.109924))
        XCTAssertEqual(points.c_d, SIMD4<Float>(60.169468, 110.45937, 58.712242, 111.28528))
        
        let points3 = r.points3
        XCTAssertEqual(points.a_b, points3.a_b)
        XCTAssertEqual(points.c_d.lowHalf, points3.c)
    }
    
    func testIntersection() {
        do {
            let r = Rect(center: SIMD2<Float>(10,10), lengths: SIMD2<Float>(1,1), angle: 0)
            let r2 = Rect(center: SIMD2<Float>(15,15), lengths: SIMD2<Float>(1,1), angle: 0)
            XCTAssert(!r.intersects(r2))
        }

        do {
            let ra =  Rect(center: SIMD2<Float>(58.95, 110.0), lengths: SIMD2<Float>(1.675, 3.85), angle: -2.0940719)
            let rb = Rect(center: SIMD2<Float>(58.95, 110.0), lengths: SIMD2<Float>(1.675, 3.85), angle: -0.5216182)
            XCTAssert(ra.intersects(rb))
        }

        do {
            //counterexample we found to previous algorithm
            let ra1 = Rect(center: SIMD2<Float>(14.661654,14.333334), lengths: SIMD2<Float>(1,1), angle: .pi)
            let ra2 = Rect(center: SIMD2<Float>(15,15), lengths: SIMD2<Float>(1,1), angle: 0)
            XCTAssert(ra1.intersects(ra2))
        }

        
        do {
            //new counterexample

            let rb1 = Rect(center: SIMD2<Float>(17.517986,16.417912), lengths: SIMD2<Float>(3.094170,1), angle: 0)
            let rb2 = Rect(center: SIMD2<Float>(15.873606,15.882353), lengths: SIMD2<Float>(1,1), angle: 0)
            XCTAssert(!rb1.intersects(rb2))
        }
        
        //new counterexample
        do {
            let rb1 = Rect(center: SIMD2<Float>(14.6999248,13.713236), lengths: SIMD2<Float>(1,1), angle: 0)
            let rb2 = Rect(center: SIMD2<Float>(13.686131,13.886792), lengths: SIMD2<Float>(1.045455,0.220588), angle: 0)
            XCTAssert(!rb1.intersects(rb2))
        }
        
    }
    

    static var allTests = [
        ("testPointInside",testPointInside),
        ("testPoints",testPoints),
        ("testPointsWithRotation",testPointsWithRotation),
        ("testIntersection",testIntersection),
    ]
}
