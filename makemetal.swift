#!/usr/bin/swift

import Foundation
let m = FileManager.default
let sourcePath = String(#file.dropLast(15))
print(sourcePath)
m.changeCurrentDirectoryPath(sourcePath) //makemetal.swift
var metalAccumulate = ""
let sourceDir = "Sources/blitcurve-c/"
for file in try m.contentsOfDirectory(atPath: sourceDir) {
    if file.hasSuffix(".c") {
        metalAccumulate += String(data: m.contents(atPath: sourceDir + file)!, encoding: .utf8)!
    }
}
let outPath = (ProcessInfo.processInfo.environment["SRCROOT"] ?? ".") + "/blitcurve.metal"
try! metalAccumulate.write(to: URL(fileURLWithPath:outPath), atomically: false, encoding:.utf8)
