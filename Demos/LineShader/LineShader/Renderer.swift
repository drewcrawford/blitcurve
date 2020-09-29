//
//  Renderer.swift
//  LineShader
//
//  Created by Drew Crawford on 9/28/20.

import Metal
import MetalKit
import simd
import os.log
import blitcurve

class Renderer: NSObject, MTKViewDelegate {
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        /* */
    }
    

    public let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var pipelineState: MTLRenderPipelineState
    let computeState: MTLComputePipelineState
    
    let cubics: MTLBuffer
    let colors: MTLBuffer
    let vectors: MTLBuffer


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
        cubics = device.makeBuffer(length: Int(CURVE_COUNT) * MemoryLayout<Cubic>.stride, options: .storageModeShared)!
        colors = device.makeBuffer(length: Int(CURVE_COUNT) * MemoryLayout<SIMD3<Float>>.stride, options: .storageModeShared)!
        vectors = device.makeBuffer(length: Int(CURVE_COUNT) * MemoryLayout<Cubic>.stride, options: .storageModeShared)!
        for i in 0..<Int(CURVE_COUNT) {
            
            let cubicPtr = cubics.contents().assumingMemoryBound(to: Cubic.self).advanced(by: i)
            let cubic = Cubic(a: SIMD2<Float>.random(in: -1...1), b: SIMD2<Float>.random(in: -1...1), c: SIMD2<Float>.random(in: -1...1), d:SIMD2<Float>.random(in: -1...1))
            cubicPtr.pointee = cubic

            
            let colorPtr = colors.contents().assumingMemoryBound(to: SIMD3<Float>.self).advanced(by: i)
            colorPtr.pointee = SIMD3<Float>.random(in: 0...1)
            
            let delta: Float = 0.01
            let vectorCubic = Cubic(a: SIMD2<Float>.random(in: -delta...delta), b: SIMD2<Float>.random(in: -delta...delta), c: SIMD2<Float>.random(in: -delta...delta), d:SIMD2<Float>.random(in: -delta...delta))
            let vectorCubicPtr = vectors.contents().assumingMemoryBound(to: Cubic.self).advanced(by: i)
            vectorCubicPtr.pointee = vectorCubic
        }
        
        super.init()
    }

    class func buildRenderPipelineWithDevice(device: MTLDevice,
                                             metalKitView: MTKView) throws -> MTLRenderPipelineState {
        /// Build a render state pipeline object

        let library = device.makeDefaultLibrary()

        let vertexFunction = library?.makeFunction(name: "vertexFunction")
        let fragmentFunction = library?.makeFunction(name: "fragmentFunction")

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
        let simulateFunction = library.makeFunction(name: "cubicMove")!
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
                computeEncoder.setBuffer(cubics, offset: 0, index: 0)

                let threadgroupSize = MTLSize(width: computeState.maxTotalThreadsPerThreadgroup, height: 1, depth: 1)
                let tgWidth = max(1, Int(ceil(Float(CURVE_COUNT) / Float(threadgroupSize.width))))
                let threadgroups = MTLSize(width: tgWidth, height: 1, depth: 1)
                computeEncoder.setBuffers([cubics,vectors], offsets: [0,0], range: 0..<2)
                var curveCount = UInt32(CURVE_COUNT)
                withUnsafeBytes(of: &curveCount) { ptr in
                    computeEncoder.setBytes(ptr.baseAddress!, length: ptr.count, index: 2)
                }
                computeEncoder.dispatchThreadgroups(threadgroups, threadsPerThreadgroup: threadgroupSize)
                computeEncoder.endEncoding()
                
                /// Final pass rendering code here
                if let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) {
                    renderEncoder.label = "Primary Render Encoder"
                    
                    
                                        
                    renderEncoder.setRenderPipelineState(pipelineState)
                    
                    
                    renderEncoder.setVertexBuffer(cubics, offset: 0, index: 0)
                    withUnsafeBytes(of: &curveCount) { ptr in
                        renderEncoder.setVertexBytes(ptr.baseAddress!, length: ptr.count, index: 1)
                    }
                    
                    renderEncoder.setVertexBuffer(colors, offset: 0, index: 2)
                    
                    renderEncoder.drawPrimitives(type: .lineStrip, vertexStart: 0, vertexCount: Int(VertexesPerCubic), instanceCount: Int(CURVE_COUNT))
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

