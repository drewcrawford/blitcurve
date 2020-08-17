//
//  MetalTest.metal
//  MetalTest
//
//  Created by Drew Crawford on 6/20/20.
//  Copyright © 2020 DrewCrawfordApps LLC. All rights reserved.
//
#include <BCCubic.h>

#include <metal_stdlib>

using namespace metal;

void foo() {
    BCCubic c;
    __attribute__((unused))
    float l = BCCubicLength(c);
}
