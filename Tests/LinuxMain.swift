// LinuxMain.swift: I make no guarantees this works on linux
// ©2020 DrewCrawfordApps LLC
import XCTest

import blitcurveTests

var tests = [XCTestCaseEntry]()
tests += blitcurveTests.allTests()
XCTMain(tests)
