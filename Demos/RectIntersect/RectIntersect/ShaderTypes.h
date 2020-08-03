//
//  ShaderTypes.h
//  RectIntersect
//
//  Created by Drew Crawford on 8/3/20.
//

//
//  Header containing types and enum constants shared between Metal shaders and Swift/ObjC source
//
#ifndef ShaderTypes_h
#define ShaderTypes_h

#include <blitcurve.h>

#include <simd/simd.h>
#if __METAL_VERSION__
constant static const uint16_t RECT_COUNT = 256;
#else
static const uint16_t RECT_COUNT = 256;
#endif

#endif /* ShaderTypes_h */

