//BCTrap.h: Trap system
// ©2020 DrewCrawfordApps LLC

/*
 Support for error handling on platforms vary widely.  On CPU platforms, we generally like trap behavior if input is very invalid, ideally with robust debugging information.  However even bare aborts are not supported in Metal, and in some of our attempts to create it we discover compiler bugs.
 
 Moreover, there is some concern about the speed vs safety of various error mechanisms.  Speed in particular varies widely across platforms, while the expected API may vary across programming environments.
 
 As such, we implement a semantic error system.  blitcurve defines a set of macros for each semantic error type, that may be customized on a per-platform basis.
 
 Generally, semantic assertions contain
 * a type (e.g. assert, precondition, etc.) This is analagous to log levels in logging systems and allow different assertions to be handled differently.
 * a condition (like a classic assert statement).  If the condition is false we do the error handling
 * an rvalue.  For cases where trap is not available, we return this value.  This may be an error code or some "reserved" value in the domain of the function, depending on the function documentation.
 
 Then a platform can customize the assert behavior, per-type.  For each type,
 
 * The platform might trap, if supported / convenient.  On Metal we have tried a variety of trap systems, not all of which work well.  At the time of this writing, we trap on CPU platforms, but this may change.
 * If not trapping, the rvalue may be returned.
 * We may in the future allow a mode for UB for errors, to allow faster performance.  Unclear to me if this will be in the form of calling special UB functions, or through a preprocessor define.
 */

#ifndef BCTrap_h
#define BCTrap_h

#ifndef __METAL_VERSION__
#include <stdio.h>
#endif

//__BC_CPU_TRAP will unconditionally trap on CPU.  No-op on GPU.
#ifdef __METAL_VERSION__
#define __BC_CPU_TRAP
#else
#define __BC_CPU_TRAP {printf("BC_CPU_TRAP %s:%d\n",__FILE__,__LINE__); exit(137);}
#endif


//Like __BC_ASSERT2, but allows custom code to run after the trap rather than using an rvalue.
#define __BC_ASSERT2_CUSTOM(CONDITION,CUSTOM) if (!__builtin_expect(CONDITION,1)) {__BC_CPU_TRAP CUSTOM;}

/*__BC_ASSERT2 traps incorrect application usage of blitcurve.  This is typically cases like invalid arguments and similar obvious misuse.
 This does not include "subtle" bugs that might only turn up in production.  For example, issues that occur only on large problem sizes.

 */
#define __BC_ASSERT2(CONDITION,RVALUE) __BC_ASSERT2_CUSTOM(CONDITION,return RVALUE)

//The BUGIF priority deals with conditions that we think are blitcurve bugs if they occur
#define __BC_BUGASSERT_CUSTOM(CONDITION,CUSTOM) if (!__builtin_expect(CONDITION,1)) {__BC_CPU_TRAP CUSTOM;}
#define __BC_BUGASSERT(CONDITION,RVALUE) __BC_BUGASSERT_CUSTOM(CONDITION,return RVALUE)

/*
 Converts some rvalue to another rvalue, for assertion-type.
 */
#define __BC_ASSERT_CONVERT(LHS,RVALUE,NEWRVALUE) if (__builtin_expect(LHS==RVALUE,0)) { __BC_CPU_TRAP return NEWRVALUE; }

/**
 This defines various error situations that might occur.  Note that in practice, functions often transform these errors in some way to fit them into values outside the function domain.
 For example, they  might be cast to floats or negated.
 */
typedef enum {
    ///Problem with the first argument.
    BCErrorArg0,
    ///Problem with the second argument.
    BCErrorArg1,
    ///Problem with the third argument.
    BCErrorArg2,
    ///Problem with relationship between multiple arguments
    BCErrorArgRelationship,
    ///A minimum and maximum value are the same, but this not allowed.
    BCErrorRangeIsZero,
    ///The size (length, etc.) of some value is 0, but this is not allowed.
    BCErrorSizeIsZero,
} BCError;

#endif
