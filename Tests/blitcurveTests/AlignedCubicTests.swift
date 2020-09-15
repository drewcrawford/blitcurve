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
        
        let t = AlignedCubic(c: SIMD2<Float>(32.121765, -14.771326), d: SIMD2<Float>(26.025661, -70.33964), b_x: 106.62551)
        XCTAssertEqual(t.kappa(t: 0.560689), 0.0449, accuracy: 0.01)
        
    }
    func testKappaBench() throws {
        #if DEBUG
        throw XCTSkip("Not running benchmark in a debug build")
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
    
    func testKappaPrime() throws {
        //this test only works in debug mode
        #if DEBUG
        
        let c = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(1000,1000), c: SIMD2<Float>(0,250), d: SIMD2<Float>(750, 0))
        let a = AlignedCubic(cubic: c)
        XCTAssertEqual(a.__kappaPrime(t: 0), -0.0826, accuracy: 0.1)
        XCTAssertEqual(a.__kappaPrime(t: 0.5), 0.003304, accuracy: 0.01)
        XCTAssertEqual(a.__kappaPrime(t: 1), -0.00211, accuracy: 0.01)
        // {ax->0,ay->0,by->0,cx->32.121765,cy->-14.771326, dx->26.025661, dy-> -70.33964, bx->106.62551, t->0.56989586}
        let a2 = AlignedCubic(c: SIMD2<Float>(32.121765, -14.771326), d: SIMD2<Float>(26.025661, -70.33964), b_x: 106.62551)
        XCTAssertEqual(a2.__kappaPrime(t: 0.56989586), -0.0296569, accuracy:0.00001)
        XCTAssertEqual(a2.__kappaPrime(t: 0.56064975), 0.000101445,accuracy:0.00001)
        #else
        throw XCTSkip("kappaPrime is not supported in release builds")
        #endif
    }
    
    func testMaxKappa() {
        
        let c2 = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(37,100), c: SIMD2<Float>(25,25), d: SIMD2<Float>(75,0))
        let a2 = AlignedCubic(cubic: c2)
        let libmax = a2.maxKappaParameter(accuracy: 0.0001)
        var t: Float? = nil
        var f: Float = 0
        for i in stride(from: 0 as Float, to: 1, by: 0.0001) {
            let a = abs(a2.kappa(t: i))
            if a > f { f = a; t = i }
        }
        XCTAssertEqual(libmax, t!,accuracy:0.0001)
        
        //try a line
        let linear = Cubic(a: SIMD2<Float>(0,0), b: SIMD2<Float>(100,100), c: SIMD2<Float>(25,25), d: SIMD2<Float>(75,75))
        let alignedLine = AlignedCubic(cubic: linear)
        let kappaT = alignedLine.maxKappaParameter(accuracy: 0.01)
        XCTAssert(kappaT >= 0 && kappaT <= 1)
        
    }
    static var allTests = [
        ("testMake", testMake),
        ("testKappaBench",testKappaBench),
        ("testKappaPrime",testKappaPrime),
        ("testMaxKappa",testMaxKappa)
    ]
}
