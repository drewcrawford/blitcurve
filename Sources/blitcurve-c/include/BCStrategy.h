// BCStrategy.h: Algorithm selection
// ©2020 DrewCrawfordApps LLC


///Indicates which of several strategies to use in a function call or algorithm

 __attribute__((swift_name("BCStrategy")))
typedef enum {
    ///Use the fastest method, regardless of its accuracy
    
    BCStrategyFastest __attribute__((swift_name("fastest")))
}  __attribute__((enum_extensibility(closed))) BCStrategy;
