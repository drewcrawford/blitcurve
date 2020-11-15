// BCMacros.h: Macros
// ©2020 DrewCrawfordApps LLC

#ifndef BCMacros_h
#define BCMacros_h

///The BC_ASSERT macro will perform a platform-agnostic assertion check.
///BC_ASSERT will be a no-op if NDEBUG is defined.  This is set either by \c  Package.swift (CPU case) or in build settings (metal case)
#ifdef NDEBUG
#define __BC_ASSERT(X)
#else

#ifndef __METAL_VERSION__
#include <assert.h>
#define __BC_ASSERT(X) assert(X)
#else
//note that on AMD, we require __builtin_trap following our assert pattern.  This seems to work around FB8897889.
#define __BC_ASSERT(X) if (__builtin_expect(!(X),0)) {float device *f = 0; *f = 0; __builtin_trap();}
#endif
#endif //NDEBUG

//__BC_MAYBESTATIC is static in release mode, or otherwise empty.
//This allows us to expose code for tests
#ifdef NDEBUG
#define __BC_MAYBESTATIC static
#else
#define __BC_MAYBESTATIC
#endif

#endif //BCMacros_h
