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
#else
#define constant
#endif
constant static const uint32_t RECT_COUNT = 8192;

#endif /* ShaderTypes_h */

