//
//  Renderer.swift
//  RectIntersect
//
//  Created by Drew Crawford on 8/3/20.
//

// Our platform independent renderer class

import Metal
import MetalKit
import simd
import os.log

class Renderer: NSObject, MTKViewDelegate {
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        /* */
    }
    

    public let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var pipelineState: MTLRenderPipelineState
    let computeState: MTLComputePipelineState
    
    var nextShapeTimer: Timer!
    var nextShapeIndex = 0
    let rects: MTLBuffer
    let vectors: MTLBuffer
    let intersects: MTLBuffer

    init?(metalKitView: MTKView) {
        self.device = metalKitView.device!
        self.commandQueue = self.device.makeCommandQueue()!
    

        metalKitView.colorPixelFormat = MTLPixelFormat.bgra8Unorm_srgb
        metalKitView.sampleCount = 1


        do {
            pipelineState = try Renderer.buildRenderPipelineWithDevice(device: device,
                                                                       metalKitView: metalKitView)
        } catch {
            print("Unable to compile render pipeline state.  Error info: \(error)")
            return nil
        }
        computeState = Renderer.buildComputePipeline(device: device)
        rects = device.makeBuffer(length: Int(RECT_COUNT) * MemoryLayout<Rect>.stride, options: .storageModeShared)!
        let rectPtr = rects.contents().assumingMemoryBound(to: Rect.self)
        rectPtr.assign(repeating: Rect(center: .zero, lengths: SIMD2<Float>(0.5,0.5), angle: 0), count: 1)
        
        vectors = device.makeBuffer(length: MemoryLayout<SIMD2<Float>>.stride * Int(RECT_COUNT), options: .storageModeShared)!
        
        intersects = device.makeBuffer(length: MemoryLayout<UInt8>.stride * Int(RECT_COUNT), options: .storageModeShared)!

        nextShapeTimer = nil
        super.init()
        generateShape()
        nextShapeTimer = Timer(timeInterval: 0.001, repeats: true, block: { [weak self] _ in
            self?.generateShape()
        })
        DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + .seconds(10)) {
            RunLoop.main.add(self.nextShapeTimer, forMode: .default)
        }
    }
    
    private func generateShape() {
        let vecPtr = vectors.contents().assumingMemoryBound(to: SIMD2<Float>.self).advanced(by: nextShapeIndex)
        var nextVector = SIMD2<Float>(Float.random(in: -0.01..<0.01), Float.random(in: -0.01..<0.01))
        withUnsafeMutablePointer(to: &nextVector) { ptr in
            vecPtr.moveAssign(from: ptr, count: 1)
        }
        
        let rectPtr = rects.contents().assumingMemoryBound(to: Rect.self).advanced(by: nextShapeIndex)
        var nextRect = Rect(center: .zero, lengths: SIMD2<Float>(Float.random(in: 0..<0.05), Float.random(in: 0..<0.05)), angle: Float.random(in: 0..<Float.pi))
        withUnsafeMutablePointer(to: &nextRect, {ptr in
            rectPtr.moveAssign(from: ptr, count: 1)
        })
        nextShapeIndex += 1
        if nextShapeIndex >= RECT_COUNT {
            nextShapeTimer.invalidate()
        }
        os_log("Shapes: %d",nextShapeIndex)
    }

    class func buildRenderPipelineWithDevice(device: MTLDevice,
                                             metalKitView: MTKView) throws -> MTLRenderPipelineState {
        /// Build a render state pipeline object

        let library = device.makeDefaultLibrary()

        let vertexFunction = library?.makeFunction(name: "vertexShader")
        let fragmentFunction = library?.makeFunction(name: "fragmentShader")

        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.label = "RenderPipeline"
        pipelineDescriptor.sampleCount = metalKitView.sampleCount
        pipelineDescriptor.vertexFunction = vertexFunction
        pipelineDescriptor.fragmentFunction = fragmentFunction

        pipelineDescriptor.colorAttachments[0].pixelFormat = metalKitView.colorPixelFormat
        pipelineDescriptor.depthAttachmentPixelFormat = metalKitView.depthStencilPixelFormat
        pipelineDescriptor.stencilAttachmentPixelFormat = metalKitView.depthStencilPixelFormat

        return try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
    }
    
    class func buildComputePipeline(device: MTLDevice) -> MTLComputePipelineState {
        let library = device.makeDefaultLibrary()!
        let simulateFunction = library.makeFunction(name: "shapeSimulator")!
        return try! device.makeComputePipelineState(function: simulateFunction)
    }



    func draw(in view: MTKView) {
        /// Per frame updates hare

        
        if let commandBuffer = commandQueue.makeCommandBuffer() {
            
            /// Delay getting the currentRenderPassDescriptor until we absolutely need it to avoid
            ///   holding onto the drawable and blocking the display pipeline any longer than necessary
            let renderPassDescriptor = view.currentRenderPassDescriptor
            
            if let renderPassDescriptor = renderPassDescriptor {
                
                let computeEncoder = commandBuffer.makeComputeCommandEncoder()!
                computeEncoder.setComputePipelineState(computeState)
                computeEncoder.setBuffer(vectors, offset: 0, index: 0)
                computeEncoder.setBuffer(intersects, offset: 0, index: 1)
                computeEncoder.setBuffer(rects, offset: 0, index: 2)
                
                let threadgroupSize = MTLSize(width: computeState.maxTotalThreadsPerThreadgroup, height: 1, depth: 1)
                let tgWidth = max(1, Int(ceil(Float(RECT_COUNT) / Float(threadgroupSize.width))))
                let threadgroups = MTLSize(width: tgWidth, height: 1, depth: 1)
                computeEncoder.dispatchThreadgroups(threadgroups, threadsPerThreadgroup: threadgroupSize)
                computeEncoder.endEncoding()
                
                /// Final pass rendering code here
                if let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) {
                    renderEncoder.label = "Primary Render Encoder"
                    
                    
                                        
                    renderEncoder.setRenderPipelineState(pipelineState)
                    
                    
                    renderEncoder.setVertexBuffer(rects, offset: 0, index: 0)
                    renderEncoder.setVertexBuffer(intersects, offset: 0, index: 1)
                    
                    renderEncoder.drawPrimitives(type: .lineStrip, vertexStart: 0, vertexCount: 5, instanceCount: Int(RECT_COUNT))
                    renderEncoder.endEncoding()
                    
                    if let drawable = view.currentDrawable {
                        commandBuffer.present(drawable)
                    }
                }
            }
            
            commandBuffer.commit()
        }
    }

}
