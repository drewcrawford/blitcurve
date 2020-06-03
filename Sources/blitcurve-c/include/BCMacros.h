// BCMacros.h: Macros
// ©2020 DrewCrawfordApps LLC

#ifndef BCMacros_h
#define BCMacros_h

#if CHECK_UB
#define ASSERT_UB(X) assert(X)
#else
#define ASSERT_UB(X)
#endif

#endif
