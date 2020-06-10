// BCMacros.h: Macros
// ©2020 DrewCrawfordApps LLC

#ifndef BCMacros_h
#define BCMacros_h

#if __METAL_VERSION__
#define BC_ASSERT(X) if (__builtin_expect(X,0)) {__builtin_trap();}
#else
#define BC_ASSERT(X) assert(X)
#endif

#if CHECK_UB
#define ASSERT_UB(X) BC_ASSERT(X)
#else
#define ASSERT_UB(X)
#endif

#endif
