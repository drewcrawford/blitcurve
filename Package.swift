// swift-tools-version:5.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "blitcurve",
    //this is here to work around FB8430635
    platforms: [.iOS("13.0")],
    products: [
        // Products define the executables and libraries produced by a package, and make them visible to other packages.
        .library(
            name: "blitcurve",
            targets: ["blitcurve"]),
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        // .package(url: /* package url */, from: "1.0.0"),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages which this package depends on.
        .target(
            name: "blitcurve",
            dependencies: ["blitcurve-c"],
        cSettings:[.define("NDEBUG", to: nil, .when(platforms: nil, configuration: .release))]
),
        .target(
            name: "blitcurve-c",
            dependencies: [],
            cSettings:[.define("NDEBUG", to: nil, .when(platforms: nil, configuration: .release))]
        ),
        .testTarget(
            name: "blitcurveTests",
            dependencies: ["blitcurve"],
            cSettings: [.define("NDEBUG",to: nil, .when(platforms: nil, configuration:.release))]
        )
    ]
)

