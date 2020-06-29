//BCCubic2.h: A vectorized BCCubic type
// ©2020 DrewCrawfordApps LLC
#import "BCTypes.h"

///\abstract Two \c BCCubic in a SIMD configuration.
///\discussion Unlike some other notation, we use \c a and \c b consistently for start and end points, reserving other values for control points.
typedef struct {
    bc_float4_t a;
    bc_float4_t b;
    bc_float4_t c;
    bc_float4_t d;
} BCCubic2;

