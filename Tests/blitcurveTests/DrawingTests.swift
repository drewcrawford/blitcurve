//DrawingTests.swift: Drawing tests
// ©2021 DrewCrawfordApps LLC

import blitcurve_c
import XCTest

class DrawingTests: XCTestCase {
    func testMakeClamped() {
        let c = Cubic(connecting: Line(point: .zero, angle: 0.2, distance: 10))
        let v1 = BCCubicVertexMakeClampedParameterization(c, 0, 4, -1, 10, 0.01, 0.1)
        XCTAssertEqual(v1, c.a)
        let v2 = BCCubicVertexMakeClampedParameterization(c, 3, 4, -1, 10, 0.01, 0.1)
        XCTAssertEqual(v2, c.b)
    }
}
