// BCAlignedRect: Axis-aligned rect
// ©2020 DrewCrawfordApps LLC
#import "BCAlignedRect.h"
extern inline bc_float2_t BCAlignedRectCenterPoint(BCAlignedRect a);
extern inline bool BCAlignedRectIsPointOnOrInside(BCAlignedRect a, bc_float2_t point);
bool BCAlignedRectsCornerWithinDistance(BCAlignedRect a, BCAlignedRect b,bc_float_t distance) {
    /*we need to compare, potentially, every point in a with every point in b.
     
     First, do the subtraction for  a: (x:min,y:min) , (x:max,y:min), against b: all
     
     Note that we put coordinates into columns
     
     ┌────┬────────────────┐
     │xmin│xmin xmin xmin  │     ┌────┬───────────────┐
     │ymin│ymin ymin ymin  │     │xmin│xmax xmin xmax │
     ├────┘                │     │ymin│ymin ymax ymax │
     │xmax xmax xmax xmax  │  -  ├────┘               │
     │ymin ymin ymin ymin  │     │xmin xmax xmin xmax │
     │                     │     │ymin ymin ymax ymax │
     └─────────────────────┘     └────────────────────┘
            A                       B
                                                       
                                                       
                                              
     */
    bc_float4x4_t mtxA = bc_make_4x4(bc_make_float4(a.min.x,a.min.y,a.max.x,a.min.y),
                                  bc_make_float4(a.min.x,a.min.y,a.max.x,a.min.y),
                                  bc_make_float4(a.min.x,a.min.y,a.max.x,a.min.y),
                                  bc_make_float4(a.min.x,a.min.y,a.max.x,a.min.y)
                                  
                                  );
    
    bc_float4x4_t mtxB = bc_make_4x4(bc_make_float4(b.min.x,b.min.y,b.min.x,b.min.y),
                                     bc_make_float4(b.max.x,b.min.y,b.max.x,b.min.y),
                                     bc_make_float4(b.min.x,b.max.y,b.min.x,b.max.y),
                                     bc_make_float4(b.max.x,b.max.y,b.max.x,b.max.y)
                                     );
    
    bc_float4x4_t mtxC = bc_sub(mtxA,mtxB);
    //I don't think it would be faster to vectorize this with simd_any
    //vs avoiding calculating the distance to begin with
    
    //on x64 anyway, simd_length_squared is a bit faster than simd_length
    bc_float_t distance_squared = distance * distance;
    if (bc_length_squared(mtxC.columns[0].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[0].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[1].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[1].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[2].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[2].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[3].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxC.columns[3].hi) < distance_squared) {
        return true;
    }
    
    /*otherwise, we need to check the other half
     
     ┌────┬────────────────┐
     │xmin│xmin xmin xmin  │     ┌────┬───────────────┐
     │ymax│ymax ymax ymax  │     │xmin│xmax xmin xmax │
     ├────┘                │     │ymin│ymin ymax ymax │
     │xmax xmax xmax xmax  │  -  ├────┘               │
     │ymax ymax ymax ymax  │     │xmin xmax xmin xmax │
     │                     │     │ymin ymin ymax ymax │
     └─────────────────────┘     └────────────────────┘
            D                       B (same as before)
                                                       
     
     */
    bc_float4x4_t mtxD = bc_make_4x4(bc_make_float4(a.min.x,a.max.y,a.max.x,a.max.y),
                                   bc_make_float4(a.min.x,a.max.y,a.max.x,a.max.y),
                                   bc_make_float4(a.min.x,a.max.y,a.max.x,a.max.y),
                                   bc_make_float4(a.min.x,a.max.y,a.max.x,a.max.y));

    bc_float4x4_t mtxE = bc_sub(mtxD, mtxB);
    
    //now a similar situation on mtxE
    
    if (bc_length_squared(mtxE.columns[0].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[0].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[1].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[1].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[2].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[2].hi) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[3].lo) < distance_squared) {
        return true;
    }
    if (bc_length_squared(mtxE.columns[3].hi) < distance_squared) {
        return true;
    }
    return false;
    
}
