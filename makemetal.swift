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

func checkPbxProj() {
    guard let srcRoot = ProcessInfo.processInfo.environment["SRCROOT"] else { return }
    guard let projectName = ProcessInfo.processInfo.environment["PROJECT_NAME"] else { return }
    guard let pbxproj = try? String(contentsOfFile: "\(srcRoot)/\(projectName).xcodeproj/project.pbxproj") else { return }
    if !pbxproj.contains("blitcurve.metal in Sources") {
        print("warning: Drag \(srcRoot)/blitcurve.metal into your xcodeproj")
    }
}

func checkHeaderPath() {
    guard let _ = ProcessInfo.processInfo.environment["XCODE_VERSION_MAJOR"] else { return }
    let searchPaths = ProcessInfo.processInfo.environment["MTL_HEADER_SEARCH_PATHS"] ?? ""
    if !searchPaths.contains("blitcurve-c/include") {
        print("warning: Add ${HEADER_SEARCH_PATHS} to the 'Metal Compiler - Build Options - Header Search Paths' setting")
    }
    print("sp",searchPaths )
}

checkPbxProj()
checkHeaderPath()
