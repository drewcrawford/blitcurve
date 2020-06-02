// operators.swift: Curve operators
// ©2020 DrewCrawfordApps LLC

infix operator ~=: ComparisonPrecedence
@_exported import blitcurve_c
extension BCFloat {
    static func ~=(lhs: BCFloat, rhs: BCFloat) -> Bool {
        IsNearlyEqual(a: lhs, b: rhs)
    }
}

extension BCFloat2 {
    static func ~=(lhs: BCFloat2, rhs: BCFloat2) -> Bool {
        IsNearlyEqual(a: lhs, b: rhs)
    }
}
