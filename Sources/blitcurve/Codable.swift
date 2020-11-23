//Codable.swift: Codable implementations
// ©2020 DrewCrawfordApps LLC

import blitcurve_c

extension Cubic: Encodable {
    fileprivate enum CodingKeys: String, CodingKey {
        case a
        case b
        case c
        case d
    }
    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(a, forKey: CodingKeys.a)
        try container.encode(b, forKey: CodingKeys.b)
        try container.encode(c, forKey: CodingKeys.c)
        try container.encode(d, forKey: CodingKeys.d)

    }
}

extension Cubic: Decodable {
    public init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let a = try container.decode(SIMD2<Float>.self, forKey: .a)
        let b = try container.decode(SIMD2<Float>.self, forKey: .b)
        let c = try container.decode(SIMD2<Float>.self, forKey: .c)
        let d = try container.decode(SIMD2<Float>.self, forKey: .d)

        self.init(a: a, b: b, c: c, d: d)
    }
}
