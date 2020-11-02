// ParameterTests.swift: Tests for BCBezierParameter
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve

class ParameterTests: XCTestCase {
    func testVertexID() {
        XCTAssertEqual(VertexToBezierParameter(vertexID: 0 as UInt16, vertexesPerInstance: 5),0)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 1 as UInt16, vertexesPerInstance: 5),0.25)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 2 as UInt16, vertexesPerInstance: 5),0.5)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 3 as UInt16, vertexesPerInstance: 5),0.75)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 4 as UInt16, vertexesPerInstance: 5),1)
    }
    
    func testVertexIDBounds() {
        XCTAssertEqual(VertexToBezierParameter(vertexID: 0 as UInt16, vertexesPerInstance: 5, lowerT: 0.5, upperT: 3.0),0.5)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 1 as UInt16, vertexesPerInstance: 5, lowerT: 0.5, upperT: 3.0),1.125)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 2 as UInt16, vertexesPerInstance: 5, lowerT: 0.5, upperT: 3.0),1.75)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 3 as UInt16, vertexesPerInstance: 5, lowerT: 0.5, upperT: 3.0),2.375)
        XCTAssertEqual(VertexToBezierParameter(vertexID: 4 as UInt16, vertexesPerInstance: 5, lowerT: 0.5, upperT: 3.0),3)
    }
    
  
    
    static var allTests = [
        ("testVertexID",testVertexID),
        ("testVertexIDBounds",testVertexIDBounds),
    ]
}

