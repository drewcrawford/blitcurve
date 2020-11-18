//
//  Test__macOS_.swift
//  Test (macOS)
//
//  Created by Drew Crawford on 11/18/20.
//  Copyright © 2020 DrewCrawfordApps LLC. All rights reserved.
//

import XCTest
import blitcurve

class Test__macOS_: XCTestCase {

    /**
     This test checks kapow, a.k.a. FB8904929.
    Although that bug includes a test case covering Intel UHD 4000, AMD 5500M, Intel UHD 630, this test does not reproduce on the latter for some reason.
     */
    func testKapow() throws {
        let device = MTLCreateSystemDefaultDevice()!
        let library = try device.makeDefaultLibrary(bundle: Bundle(for: Test__macOS_.self))
        let function = library.makeFunction(name: "kapow")!
        let pso = try device.makeComputePipelineState(function: function)
        
        let commandQ = device.makeCommandQueue()!
        let commandBuffer = commandQ.makeCommandBuffer()!
        let commandEncoder = commandBuffer.makeComputeCommandEncoder()!
        commandEncoder.setComputePipelineState(pso)
        let cubic = Cubic(a: SIMD2<Float>(10,10), b: SIMD2<Float>(10,-10), c: SIMD2<Float>(15,10),d: SIMD2<Float>(5,-10))
        withUnsafeBytes(of: cubic) { ptr in
            commandEncoder.setBytes(ptr.baseAddress!, length: ptr.count, index: 0)
        }
        let t: Float = 0.5
        withUnsafeBytes(of: t) { ptr in
            commandEncoder.setBytes(ptr.baseAddress!, length: ptr.count, index: 1)
        }
        let outBuffer = device.makeBuffer(length: 1024, options: .storageModeShared)!
        commandEncoder.setBuffer(outBuffer, offset: 0, index: 2)
        
        commandEncoder.dispatchThreadgroups(MTLSize(width: 1, height: 1, depth: 1), threadsPerThreadgroup: MTLSize(width: 1, height: 1, depth: 1))
        commandEncoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()
        
        let leftSplit = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 1).pointee
        let expectedSplit = cubic.leftSplit(t: t)
        
        XCTAssertEqual(leftSplit.a, expectedSplit.a)
        XCTAssertEqual(leftSplit.b, expectedSplit.b)
        XCTAssertEqual(leftSplit.c, expectedSplit.c)
        XCTAssertEqual(leftSplit.d, expectedSplit.d)
        
        let rightSplit = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 2).advanced(by: 1).pointee
        let expectedRightSplit = cubic.rightSplit(t: t)

        XCTAssertEqual(rightSplit.a, expectedRightSplit.a)
        XCTAssertEqual(rightSplit.b, expectedRightSplit.b)
        XCTAssertEqual(rightSplit.c, expectedRightSplit.c)
        XCTAssertEqual(rightSplit.d, expectedRightSplit.d)
        
        let fullSplitLeft = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 4).advanced(by: 2).pointee
        let fullSplitRight = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 4).advanced(by: 3).pointee
        
        XCTAssertEqual(fullSplitLeft.a, expectedSplit.a)
        XCTAssertEqual(fullSplitLeft.b, expectedSplit.b)
        XCTAssertEqual(fullSplitLeft.c, expectedSplit.c)
        XCTAssertEqual(fullSplitLeft.d, expectedSplit.d)

        XCTAssertEqual(fullSplitRight.a, expectedRightSplit.a)
        XCTAssertEqual(fullSplitRight.b, expectedRightSplit.b)
        XCTAssertEqual(fullSplitRight.c, expectedRightSplit.c)
        XCTAssertEqual(fullSplitRight.d, expectedRightSplit.d)
        
        //check the situation for t
        
        let leftSplit_4 = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 8).advanced(by: 4).pointee
        let rightSplit_5 = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 8).advanced(by: 5).pointee
        let leftSplit_6 = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 8).advanced(by: 6).pointee
        let rightSplit_7 = outBuffer.contents().bindMemory(to: Cubic.self, capacity: 8).advanced(by: 7).pointee

        XCTAssertEqual(leftSplit_4.a, expectedSplit.a)
        XCTAssertEqual(leftSplit_4.b, expectedSplit.b)
        XCTAssertEqual(leftSplit_4.c, expectedSplit.c)
        XCTAssertEqual(leftSplit_4.d, expectedSplit.d)
        
        XCTAssertEqual(rightSplit_5.a, expectedRightSplit.a)
        XCTAssertEqual(rightSplit_5.b, expectedRightSplit.b)
        XCTAssertEqual(rightSplit_5.c, expectedRightSplit.c)
        XCTAssertEqual(rightSplit_5.d, expectedRightSplit.d)
        
        XCTAssertEqual(leftSplit_6.a, expectedSplit.a)
        XCTAssertEqual(leftSplit_6.b, expectedSplit.b)
        XCTAssertEqual(leftSplit_6.c, expectedSplit.c)
        XCTAssertEqual(leftSplit_6.d, expectedSplit.d)
        
        XCTAssertEqual(rightSplit_7.a, expectedRightSplit.a)
        XCTAssertEqual(rightSplit_7.b, expectedRightSplit.b)
        XCTAssertEqual(rightSplit_7.c, expectedRightSplit.c)
        XCTAssertEqual(rightSplit_7.d, expectedRightSplit.d)

    }

}
