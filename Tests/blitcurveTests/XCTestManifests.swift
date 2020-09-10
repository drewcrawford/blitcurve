// XCTestManifests.swift: Test manifest
// ©2020 DrewCrawfordApps LLC
import XCTest

#if !canImport(ObjectiveC)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(LineTests.allTests),
        testCase(AlignedRectTests.allTests),
        testCase(RectTests.allTests),
        testCase(CubicTests.allTests),
        testCase(LineTests.allTests),
        testCase(ParameterTests.allTests),
        testCase(AlignedCubicTests.allTests),
    ]
}
#endif
