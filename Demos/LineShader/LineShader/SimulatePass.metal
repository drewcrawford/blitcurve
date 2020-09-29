//
//  SimulatePass.metal
//  LineShader
//
//  Created by Drew Crawford on 9/28/20.
//

#include <blitcurve.h>
using namespace metal;

kernel void cubicMove(const uint position [[thread_position_in_grid]], device BCCubic *cubics, device BCCubic *cubicVectors, constant const ushort &maxCubic) {
    if (position >= maxCubic) {
        return;
    }
    BCCubic vector = cubicVectors[position];
    BCCubic cubic = cubics[position];
    cubic.a += vector.a;
    cubic.b += vector.b;
    cubic.c += vector.c;
    cubic.d += vector.d;
    cubics[position] = cubic;
    if (cubic.a.x > 1 || cubic.a.y >1 || cubic.a.x < -1 || cubic.a.x < -1) {
        vector.a *= -1;
    }
    if (cubic.b.x > 1 || cubic.b.y >1 || cubic.b.x < -1 || cubic.b.x < -1) {
        vector.b *= -1;
    }
    if (cubic.c.x > 1 || cubic.c.y >1 || cubic.c.x < -1 || cubic.c.x < -1) {
        vector.c *= -1;
    }
    if (cubic.d.x > 1 || cubic.d.y >1 || cubic.d.x < -1 || cubic.d.x < -1) {
        vector.d *= -1;
    }
    cubicVectors[position] = vector;
}


