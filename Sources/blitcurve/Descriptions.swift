//Descriptions.swift: Swift CustomStringConvertible implementations
// ©2020 DrewCrawfordApps LLC

import blitcurve_c

extension Cubic: CustomStringConvertible {
    public var description: String {
        return "Cubic(a: BCFloat2(x: \(a.x), y: \(a.y)), b: BCFloat2(x: \(b.x), y: \(b.y)), c: BCFloat2(x: \(c.x), y: \(c.y)), d: BCFloat2(x: \(d.x), y: \(d.y)))"
    }
}
