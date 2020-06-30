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
#import <assert.h>
#define __BC_ASSERT(X) assert(X)
#else
#define __BC_ASSERT(X) if (__builtin_expect(!(X),0)) {float device *f = 0; *f = 0;}
#endif

#endif //NDEBUG

#endif //BCMacros_h
