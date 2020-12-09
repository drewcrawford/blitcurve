//Equatable.swift: Equatable implementation
// ©2020 DrewCrawfordApps LLC

import blitcurve_c

extension Cubic: Equatable {
    public static func ==(lhs: Cubic, rhs: Cubic) -> Bool {
        lhs.a==rhs.a && lhs.b==rhs.b && lhs.c==rhs.c && lhs.d==rhs.d
    }
}
