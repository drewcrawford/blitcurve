//
//  MetalTest.metal
//  MetalTest
//
//  Created by Drew Crawford on 6/20/20.
//  Copyright © 2020 DrewCrawfordApps LLC. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;
#include <BCCubic.h>



void foo() {
    BCCubic c;
    __attribute__((unused))
    float l = BCCubicLength(c);
}
