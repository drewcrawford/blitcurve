//AlignedCubicTests.swift: Aligned cubic tests
// ©2020 DrewCrawfordApps LLC

import XCTest
import blitcurve_c

final class AlignedCubicTests: XCTestCase {
    func testMake() {
        let c = Cubic(a: SIMD2<Float>(120,160), b: SIMD2<Float>(220,40), c: SIMD2<Float>(35,200), d: SIMD2<Float>(220,260))
        let alignedCubic = AlignedCubic(cubic: c)
        XCTAssertEqual(alignedCubic.b_x, 156,accuracy:1)
        XCTAssertLessThan(simd_distance(alignedCubic.c, SIMD2<Float>(-85,-40)), 2)
        XCTAssertLessThan(simd_distance(alignedCubic.d, SIMD2<Float>(SIMD2<Float>(-12,140))), 2)
    }
    
    func testKappa() {
        //basically a line
        let basicallyALine = Cubic(connecting: Line(a: .zero, b: SIMD2<Float>(100,100)))
        let alignedLine = AlignedCubic(cubic: basicallyALine)
        XCTAssertEqual(alignedLine.kappa(t: 0.5),0, accuracy: 0.01)
        
        //fromQuad
        let fromQuad = Cubic(a: .zero, b: SIMD2<Float>(100,100), c: SIMD2<Float>(66.66667, 0), d: SIMD2<Float>(100,33.3333))
        let alignedFromQuad = AlignedCubic(cubic: fromQuad)
        XCTAssertEqual(alignedFromQuad.kappa(t: 0.5), 0.0147, accuracy: 0.001)
        
        let small = Cubic(a: SIMD2<Float>(619.19244,913.3555), b: SIMD2<Float>(888.6296,1392.5944), c: SIMD2<Float>(709.89105,1074.6781), d: SIMD2<Float>(799.70337,1234.4243))
        let alignedSmall = AlignedCubic(cubic: small)
        XCTAssertLessThan(alignedSmall.kappa(t: 0.07617), 0.00001)
    }
    func testKappaBench() throws {
        #if DEBUG
        throw XCTSkip()
        #else
        let small = Cubic(a: SIMD2<Float>(619.19244,913.3555), b: SIMD2<Float>(888.6296,1392.5944), c: SIMD2<Float>(709.89105,1074.6781), d: SIMD2<Float>(799.70337,1234.4243))
        let alignedSmall = AlignedCubic(cubic: small)
        
        var floats: [Float] = []
        for _ in 0..<10000000 {
            floats.append(Float.random(in: 0...1))
        }
        

        var i: UInt32 = 0
        measure {
            for float in floats {
                i ^= alignedSmall.kappa(t: float).bitPattern
            }
        }
        print(i)
        #endif

    }
    static var allTests = [
        ("testMake", testMake),
        ("testKappaBench",testKappaBench)
    ]
}
