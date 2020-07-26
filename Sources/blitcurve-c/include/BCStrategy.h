// BCStrategy.h: Algorithm selection
// ©2020 DrewCrawfordApps LLC

#ifndef BCStrategy_h
#define BCStrategy_h
 __attribute__((swift_name("BCStrategy")))
///Indicates which of several strategies to use in a function call or algorithm
typedef enum {
    ///Use the fastest method, regardless of its accuracy
    BCStrategyFastest __attribute__((swift_name("fastest")))
}  __attribute__((enum_extensibility(closed))) BCStrategy;
#endif
