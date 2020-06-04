// operators.swift: Curve operators
// ©2020 DrewCrawfordApps LLC

infix operator =~=: ComparisonPrecedence
@_exported import blitcurve_c
extension BCFloat {
    public static func =~=(lhs: BCFloat, rhs: BCFloat) -> Bool {
        IsNearlyEqual(a: lhs, b: rhs)
    }
}

extension BCFloat2 {
    public static func =~=(lhs: BCFloat2, rhs: BCFloat2) -> Bool {
        print("will check \(lhs) against \(rhs)")
        return IsNearlyEqual(a: lhs, b: rhs)
    }
}
