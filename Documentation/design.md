#  Context and scope

blitcurve is a C/C++ library for bezier curves and similar 2D geometric primitives, including common algorithms.  
These mathematical objects have applications in games, animations, physical behaviors, simulations, etc.  

Comparable libraries in this space include:

* [bezier.js](https://pomax.github.io/bezierjs/)
* [bezier.dart](https://pub.dev/packages/bezier)
* [lyon_geom](https://docs.rs/lyon_geom/0.15.3/lyon_geom/)


# Goals and non-goals

blitcurve
* is written in modern C, and also compiles for C++
* contains algorithms suitable for both drawing and for general-purpose calculation
* is small and fast
* is used by the author primarily for native iOS and macOS apps
* uses Apple's `simd` framework for vectorization, but is otherwise portable
* provides first-class Swift overlays for ergonomic use from that language, including modern conveniences like SwiftUI visualizations.
* cross-compiles to Metal and can be run entirely on-GPU.  In this configuration it is *extremely* fast; many orders of magnitude faster than other 2D curve approaches like CoreGraphics, UIKit, or SwiftUI.

It is not:
* a complete drawing solution.  You can use blitcurve as an input to your platform drawing system, such as UIKit, SwiftUI, CoreGraphics, or a Metal shader, but it doesn't replace it.
* an engine.  You just create objects and perform operations on them, and get results.  There is no global context, timestep, session, world, etc.
* a substitute for mathematical knowledge.  blitcurve provides common algorithms but you have to know if any of them apply to your problem.

# Design

## coordinate space

blitcurve does not really define an origin space or direction, so how you position `-1, 1` in relation to `0,0` is up to you.  The SwiftUI visualizations use SwiftUI's default coordinate space.

some operations may necessarily imply a certain directionality or relationship between points.  For more details, see the particular operations.

## types

By default, blitcurve operates in 32-bit floating precision.  In the future, this may be extended to other alternatives.

blitcurve consists primarily several types, which are implemented as structs, and functions involving those structs.  Additional types may be added.

blitcurve in C/C+ uses the `BC` prefix for its symbols.  note that these APIs are refined for Swift, and this prefix is dropped, so `BCCubic` is simply `Cubic` in Swift, etc.  This convention may be analagous to the `NS` prefix in Swift.

## `BCCubic`

`BCCubic` models a cubic bezier curve as two endpoints and two control points.
In blitcurve, we use the following convention for naming points:
* `a`:  start point
* `b`: end point
* `c`: control point 1
* `d`: control point 2

`BCCubic` has various algorithms you can read about in [BCCubic.h](../Sources/blitcurve-c/include/BCCubic.h), but briefly:

* Finding initial and final tangent lines
* Evaluating points on the cubic at some `t`, and on related curves like the derivative
* Evaluating tangents of the curve
* Normalizing curves.  Certain operations are not defined for non-normal curves, see headers for details
* splitting curves
* Various ways to create curves, such as by connecting other curves
* Arclength parametrization (finding points a given distance along a curve)
* others


## `BCLine`
A line type.  This provides a subset of the algorithms for `BCCubic`.

## `BCRect`
A rectangle (non-axis-aligned).  This can be used for bounding-box-like applications and other general-purpose geometric applications.

This is defined with a center, size, and angle.  Other representations can be accessed, but e.g. not all 4 points you create would be rectangles, so you cannot create `BCRect` from them.

## `BCAlignedRect`
An axis-aligned rect.  This can be used for bounding-box-like applications and general-purpose geometric applications.

This is defined with points for minimum and maximum.

This document only describes the major types, for a full list of types check the [headers](../Sources/blitcurve-c/include).

Future work: quadratic curves.

# Examples
For examples of using blitcurve and interactive demos, see the [Playgrounds](../Playgrounds) folder.

# Degree of constraint

blitcurve is originally conceived and maintained as a library to power some other iOS / macOS projects.  Inevitably some requirements of those projects get tangled up in here.

Technically, the build system needs to integrate well with iOS/macOS build systems, and work on the same compilers and in the same environments I use elsewhere.  The majority of the library needs to be written in the portable subset C that also compiles to Metal, with [some ifdefs](../Sources/blitcurve-c/include/BCMetalC.h) to smooth out the rough edges.

Algorithmically, it needs the primitives needed to power the applications, which are really application-specific, and may be drawing related or general geometry.  Often, numerical approximations with a tuneable tradeoff are chosen, or approaches known to generate good code on Apple Silicon.

On the CPU-target side, blitcurve requires reasonably vectorized implementations, first-class Swift API overlays, and support for debugging so problems in complex scenes can be quickly identified.
On the GPU-target side, blitcurve needs to avoid unnecessary memory stalls, register spills, and other GPU-specific gotchas.  Longer-term, the GPU API may need to be extended to support half-precision calculations.

# Alternatives considered

Originally, I used CoreGraphics for drawing.  As my need for general geometric algorithms increased, I developed a Swift-language version of blitcurve.

However, these approaches did not scale to the level of performance I required.  It became clear that I needed to migrate at least some geometric code to GPU, and blitcurve was born as a portable CPU/GPU solution.




