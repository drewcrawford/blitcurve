// XCTestManifests.swift: Test manifest
// ©2020 DrewCrawfordApps LLC
import XCTest

#if !canImport(ObjectiveC)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(LineTests.allTests),
    ]
}
#endif
