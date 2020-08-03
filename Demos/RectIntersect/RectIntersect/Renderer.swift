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

private let RECT_COUNT = 256

class Renderer: NSObject, MTKViewDelegate {
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        /* */
    }
    

    public let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var pipelineState: MTLRenderPipelineState
    let rects: MTLBuffer

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
        rects = device.makeBuffer(length: RECT_COUNT * MemoryLayout<Rect>.stride, options: .storageModeShared)!
        
        let rectPtr = rects.contents().assumingMemoryBound(to: Rect.self)
        rectPtr.assign(repeating: Rect(center: .zero, lengths: SIMD2<Float>(0.5,0.5), angle: 0), count: 1)
        
        super.init()

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



    func draw(in view: MTKView) {
        /// Per frame updates hare

        
        if let commandBuffer = commandQueue.makeCommandBuffer() {
            
            /// Delay getting the currentRenderPassDescriptor until we absolutely need it to avoid
            ///   holding onto the drawable and blocking the display pipeline any longer than necessary
            let renderPassDescriptor = view.currentRenderPassDescriptor
            
            if let renderPassDescriptor = renderPassDescriptor {
                
                /// Final pass rendering code here
                if let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) {
                    renderEncoder.label = "Primary Render Encoder"
                    
                    
                                        
                    renderEncoder.setRenderPipelineState(pipelineState)
                    
                    var instanceCount: UInt16 = UInt16(RECT_COUNT)
                    withUnsafeBytes(of: &instanceCount) { (ptr) -> () in
                        renderEncoder.setVertexBytes(ptr.baseAddress!, length: ptr.count, index: 0)
                    }
                    
                    renderEncoder.setVertexBuffer(rects, offset: 0, index: 1)
                    
                    renderEncoder.drawPrimitives(type: .lineStrip, vertexStart: 0, vertexCount: 5, instanceCount: RECT_COUNT)
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
