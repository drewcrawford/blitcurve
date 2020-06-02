import XCTest
@testable import blitcurve

final class blitcurveTests: XCTestCase {
    func testExample() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.
        XCTAssertEqual(blitcurve().text, "Hello, World!")
    }

    static var allTests = [
        ("testExample", testExample),
    ]
}
