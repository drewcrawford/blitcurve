//XCTSIMD.swift: SIMD extensions to XCTest
// ©2020 DrewCrawfordApps LLC

//see FB8735208
import XCTest
import simd
func XCTAssertEqual<T>(_ expression1: @autoclosure () throws -> T, _ expression2: @autoclosure () throws -> T, accuracy: Float, _ message: @autoclosure () -> String = "", file: StaticString = #filePath, line: UInt = #line) where T: SIMD, T.Scalar == Float {
    do {
        let e = try expression1()
        let f = try expression2()
        let d = e - f
        if abs(d.sum()) > accuracy {
            XCTFail("\(message()): \(e) is not equal to \(f) within \(accuracy) (differs by \(d))")
        }
    }
    catch {
        XCTFail("\(error) \(message()) \(file) \(line)")
    }
}
