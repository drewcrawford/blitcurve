// BCMacros.h: Macros
// ©2020 DrewCrawfordApps LLC

#ifndef BCMacros_h
#define BCMacros_h

//__BC_MAYBESTATIC is static in release mode, or otherwise empty.
//This allows us to expose code for tests
#ifdef NDEBUG
#define __BC_MAYBESTATIC static
#else
#define __BC_MAYBESTATIC
#endif

#endif //BCMacros_h
