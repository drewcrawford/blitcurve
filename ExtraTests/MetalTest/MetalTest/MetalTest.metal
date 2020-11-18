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

float opaqueHalf();

kernel void kapow(device const BCCubic *input, device const float *t, device BCCubic *output) {
    
    //the first four values use an opaque t
    BCCubic c = *input;
    output[0] = BCCubicLeftSplit(c, opaqueHalf());
    output[1] = BCCubicRightSplit(c, opaqueHalf());
    
    BCCubic2 fullSplit = BCCubicSplit(c, opaqueHalf());
    output[2].a = fullSplit.a.xy;
    output[2].b = fullSplit.b.xy;
    output[2].c = fullSplit.c.xy;
    output[2].d = fullSplit.d.xy;

    output[3].a = fullSplit.a.zw;
    output[3].b = fullSplit.b.zw;
    output[3].c = fullSplit.c.zw;
    output[3].d = fullSplit.d.zw;

    //repeat the test with *t instead of opaqueHalf
    output[4] = BCCubicLeftSplit(c, *t);
    output[5] = BCCubicRightSplit(c, opaqueHalf());
    
    BCCubic2 fullSplit2 = BCCubicSplit(c, *t);
    output[6].a = fullSplit2.a.xy;
    output[6].b = fullSplit2.b.xy;
    output[6].c = fullSplit2.c.xy;
    output[6].d = fullSplit2.d.xy;

    output[7].a = fullSplit2.a.zw;
    output[7].b = fullSplit2.b.zw;
    output[7].c = fullSplit2.c.zw;
    output[7].d = fullSplit2.d.zw;
    
}
