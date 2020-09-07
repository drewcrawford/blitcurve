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
    
    func testLUTParameter() {
        //range 0,1
        XCTAssertEqual(LUTParameter(index: 0, capacity: 2),0)
        XCTAssertEqual(LUTParameter(index: 1, capacity: 2),1)
        
        XCTAssertEqual(LUTParameter(index: 0, capacity: 3),0)
        XCTAssertEqual(LUTParameter(index: 1, capacity: 3),0.5)
        XCTAssertEqual(LUTParameter(index: 2, capacity: 3), 1)
        
        XCTAssertEqual(LUTParameter(index: 0, capacity: 4),0)
        XCTAssertEqual(LUTParameter(index: 1, capacity: 4),0.33, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 2, capacity: 4), 0.66, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 3, capacity: 4), 1)
        
        //special range 0-0.5
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 2, lowerT: 0, upperT: 0.5), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 2, lowerT: 0, upperT: 0.5), 0.5)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 3, lowerT: 0, upperT: 0.5), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 3, lowerT: 0, upperT: 0.5), 0.25)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 3, lowerT: 0, upperT: 0.5), 0.5)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 4, lowerT: 0, upperT: 0.5), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 4, lowerT: 0, upperT: 0.5), 0.16, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 4, lowerT: 0, upperT: 0.5), 0.33, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 3, LUTCapacity: 4, lowerT: 0, upperT: 0.5), 0.5)
        
        //special range 0-0.1
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 2, lowerT: 0, upperT: 0.1), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 2, lowerT: 0, upperT: 0.1), 0.1)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 3, lowerT: 0, upperT: 0.1), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 3, lowerT: 0, upperT: 0.1), 0.05)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 3, lowerT: 0, upperT: 0.1), 0.1)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 4, lowerT: 0, upperT: 0.1), 0)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 4, lowerT: 0, upperT: 0.1), 0.03, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 4, lowerT: 0, upperT: 0.1), 0.06, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 3, LUTCapacity: 4, lowerT: 0, upperT: 0.1), 0.1)
        
        //special range 0.5-0.75
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 2, lowerT: 0.5, upperT: 0.75), 0.5)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 2, lowerT: 0.5, upperT: 0.75), 0.75)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 3, lowerT: 0.5, upperT: 0.75), 0.5)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 3, lowerT: 0.5, upperT: 0.75), 0.625)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 3, lowerT: 0.5, upperT: 0.75), 0.75)
        
        XCTAssertEqual(LUTParameter(index: 0, LUTCapacity: 4, lowerT: 0.5, upperT: 0.75), 0.5)
        XCTAssertEqual(LUTParameter(index: 1, LUTCapacity: 4, lowerT: 0.5, upperT: 0.75), 0.583, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 2, LUTCapacity: 4, lowerT: 0.5, upperT: 0.75), 0.66, accuracy: 0.01)
        XCTAssertEqual(LUTParameter(index: 3, LUTCapacity: 4, lowerT: 0.5, upperT: 0.75), 0.75)
    }
    
    func testLUTIndex() {
        let roundingMode = LUTRoundingMode.closest
        //range 0,1
        XCTAssertEqual(LUTIndex(t: 0, capacity: 2, roundingMode: roundingMode),0)
        XCTAssertEqual(LUTIndex(t: 1, capacity: 2, roundingMode: roundingMode),1)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 3, roundingMode: roundingMode),0)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: roundingMode),1)
        XCTAssertEqual(LUTIndex(t: 1, capacity: 3, roundingMode: roundingMode), 2)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 4, roundingMode: roundingMode),0)
        XCTAssertEqual(LUTIndex(t: 0.33, capacity: 4, roundingMode: roundingMode),1)
        XCTAssertEqual(LUTIndex(t: 0.66, capacity: 4, roundingMode: roundingMode),2)
        XCTAssertEqual(LUTIndex(t: 1, capacity: 4, roundingMode: roundingMode),3)
        
        //special range 0-0.5
        XCTAssertEqual(LUTIndex(t: 0, capacity: 2, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),0)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 2, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),1)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),0)
        XCTAssertEqual(LUTIndex(t: 0.25, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),1)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.5), 2)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),0)
        XCTAssertEqual(LUTIndex(t: 0.16, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),1)
        XCTAssertEqual(LUTIndex(t: 0.33, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),2)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.5),3)
        
        //special range 0-0.1
        XCTAssertEqual(LUTIndex(t: 0, capacity: 2, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),0)
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 2, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),1)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),0)
        XCTAssertEqual(LUTIndex(t: 0.05, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),1)
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 3, roundingMode: roundingMode, lowerT: 0, upperT: 0.1), 2)
        
        XCTAssertEqual(LUTIndex(t: 0, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),0)
        XCTAssertEqual(LUTIndex(t: 0.03, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),1)
        XCTAssertEqual(LUTIndex(t: 0.06, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),2)
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 4, roundingMode: roundingMode, lowerT: 0, upperT: 0.1),3)
        
        //special range 0.5-0.75
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 2, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.75, capacity: 2, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),1)
        
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.625, capacity: 3, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.75, capacity: 3, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75), 2)
        
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 4, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.583, capacity: 4, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.66, capacity: 4, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),2)
        XCTAssertEqual(LUTIndex(t: 0.75, capacity: 4, roundingMode: roundingMode, lowerT: 0.5, upperT: 0.75),3)
        
        //check rounding modes
        //this case is exact
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: .closest),1)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: .least),1)
        XCTAssertEqual(LUTIndex(t: 0.5, capacity: 3, roundingMode: .most),1)
        
        //another exact case
        XCTAssertEqual(LUTIndex(t: 0.625, capacity: 3, roundingMode: .closest, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.625, capacity: 3, roundingMode: .least, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.625, capacity: 3, roundingMode: .most, lowerT: 0.5, upperT: 0.75),1)

        //in this case, we are a bit low
        XCTAssertEqual(LUTIndex(t: 0.4, capacity: 3, roundingMode: .closest),1)
        XCTAssertEqual(LUTIndex(t: 0.4, capacity: 3, roundingMode: .least),0)
        XCTAssertEqual(LUTIndex(t: 0.4, capacity: 3, roundingMode: .most),1)
        
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .closest, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .least, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .most, lowerT: 0.5, upperT: 0.75),1)
        
        //in this case, we are a bit high
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .closest),1)
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .least),1)
        XCTAssertEqual(LUTIndex(t: 0.6, capacity: 3, roundingMode: .most),2)
        
        XCTAssertEqual(LUTIndex(t: 0.63, capacity: 3, roundingMode: .closest, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.63, capacity: 3, roundingMode: .least, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.63, capacity: 3, roundingMode: .most, lowerT: 0.5, upperT: 0.75),2)
        
        //very low
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 3, roundingMode: .closest),0)
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 3, roundingMode: .least),0)
        XCTAssertEqual(LUTIndex(t: 0.1, capacity: 3, roundingMode: .most),1)
        
        XCTAssertEqual(LUTIndex(t: 0.52, capacity: 3, roundingMode: .closest, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.52, capacity: 3, roundingMode: .least, lowerT: 0.5, upperT: 0.75),0)
        XCTAssertEqual(LUTIndex(t: 0.52, capacity: 3, roundingMode: .most, lowerT: 0.5, upperT: 0.75),1)
        
        //very  high
        XCTAssertEqual(LUTIndex(t: 0.99, capacity: 3, roundingMode: .closest),2)
        XCTAssertEqual(LUTIndex(t: 0.99, capacity: 3, roundingMode: .least),1)
        XCTAssertEqual(LUTIndex(t: 0.99, capacity: 3, roundingMode: .most),2)
        
        XCTAssertEqual(LUTIndex(t: 0.74, capacity: 3, roundingMode: .closest, lowerT: 0.5, upperT: 0.75),2)
        XCTAssertEqual(LUTIndex(t: 0.74, capacity: 3, roundingMode: .least, lowerT: 0.5, upperT: 0.75),1)
        XCTAssertEqual(LUTIndex(t: 0.74, capacity: 3, roundingMode: .most, lowerT: 0.5, upperT: 0.75),2)

    }
    
    static var allTests = [
        ("testVertexID",testVertexID),
        ("testVertexIDBounds",testVertexIDBounds),
        ("testLUTParameter",testLUTParameter),
        ("testLUTIndex",testLUTIndex),
    ]
}

