// BCCubic.c: Cubic implementation
// ©2020 DrewCrawfordApps LLC

#include "BCCubic.h"

bc_float_t BCCubicLength(BCCubic c) {
    /*as far as I'm aware, the implementation here has some unique properties I have not seen in other bezier-curve sources.
    the core idea here is a Lobatto quadrature for n=5, to integrate the length-of-derivative.  That constant-time solution is well-known in the literature, although not widely used from what I can see.
     https://mathworld.wolfram.com/LobattoQuadrature.html
     
     The innovation here is to reimagine the quadrature in a matrix notation, for which fast vectorized primitves are available.  The result is a vectorized implementation which is faster than the usual methods.
     
    
        for v1,v3 we want to compute some equation like
    v1.x = abs(-0.558983582205757*c0.x + 0.325650248872424*c1.x + 0.208983582205757*c2.x + 0.024349751127576*c3.x)
    v3.x abs(-0.024349751127576*c0.x - 0.208983582205757*c1.x - 0.325650248872424*c2.x + 0.558983582205757*c3.x)
     
     all where c0=a,c1=c,c2=d,c3=b
     let's call coefficiencts j*c0 + k*c1 + l*c2 + m * c3
    The part inside the abs can be represented as a matrix multiplication with
                                   ┌────────┐
           ┌─────────────────┐     │j1,j3   │      ┌────────┐
           │c0x,c1x,c2x,c3x  │     │k1,k3   │  =   │v1x,v3x │
           │c0y,c1y,c2y,c3y  │  x  │l1,l3   │      │v1y,v3y │
           └─────────────────┘     │ m1,m3  │      └────────┘
                                   │        │
                 A                 └────────┘
                                                    C
                                      B
     Note that simd uses a column-major oder

     */
    simd_float4 v1_coef = simd_make_float4(-0.558983582205757, 0.325650248872424, 0.208983582205757,0.024349751127576);
    simd_float4 v3_coef = simd_make_float4(-0.024349751127576, -0.208983582205757, -0.325650248872424, 0.558983582205757);
    
    simd_float4x2 mtxA = bc_make_4x2(c.a, c.c,c.d,c.b);
    simd_float2x4 mtxB = bc_make_2x4(v1_coef, v3_coef);

    simd_float2x2 mtxC = simd_mul(mtxA, mtxB);
    
    
    /*we could potentially use a similar trick to calculate the simpler expressions
     v2_x = abs(c3.x-c0.x+c2.x-c1.x)*0.26666666666666666
    v0_x = abs(c1.x-c0.x)*0.15
    and
    v4_x = abs(c3.x-c2.x) * 0.15
     
     However v0 and v4 only refer to 2 coefficients each, so we are wasting a bit.  Also we can implement this without any multiplication via this method
      0    1   2   3       0    1   2  3           0   1    2    3
     ┌────────────────┐   ┌───────────────┐      ┌───────────────────┐
     │c1x c3x c3x c2x │   │c0x c2x c0x c1x│      | v0x v4x v21x v22x │
     │c1y c3y c3y c2y │ - │c0y c1y c0x c1y│ =    │ v0y v4y v22y v22y │
     └────────────────┘   └───────────────┘      │                   │
                                                 └───────────────────┘
     D                      E                     F
    */
    
    simd_float4x2 mtxD = bc_make_4x2(c.c, c.b, c.b, c.d);
    simd_float4x2 mtxE = bc_make_4x2(c.a, c.d, c.a, c.c);
    simd_float4x2 mtxF = simd_sub(mtxD, mtxE);
    
    //to complete v2, we need to get v21 + v22
    simd_float2 v2_needsabs_unmultiplied = mtxF.columns[2] + mtxF.columns[3];
    
    /*at this point, we need to calculate our absolute values
    across v0,v1,v2,v3,v4 x 2 dimensions (10 in total)
     
     
     We assemble these in a strange order.  v0,v2,v4 require some additional multiplication.
     v0,v4 will be 0.15
     v2 will be 0.26...
     
     while v1,v3 will be complete after abs.
    */
    simd_float4 a_v0_v4 = simd_make_float4(mtxF.columns[0],mtxF.columns[1]);
    simd_float4 a_v2_v1 = simd_make_float4(v2_needsabs_unmultiplied,mtxC.columns[0]);
    simd_float2 a_v3 = mtxC.columns[1];
    
    //note: simd_float8/16 is not available in Metal
    //need a 4-wide abs solution for that case, we're just assembling and taking the abs of these 10 values
#ifndef BC_NO_VEC8
    simd_float8 a8_v0_v4_v2_v1 = simd_make_float8(a_v0_v4,a_v2_v1);
    
    simd_float8 takenAbs_v0_v4_v2_v1 = simd_abs(a8_v0_v4_v2_v1);
    //get v1,v3 out
    //these terms are fully baked
    simd_float2 v1 = takenAbs_v0_v4_v2_v1.hi.hi;
#else

    simd_float4 takenAbs_v0_v4 = simd_abs(a_v0_v4);
    simd_float4 takenAbs_v2_v1 = simd_abs(a_v2_v1);
    simd_float2 v1 = takenAbs_v2_v1.hi;
#endif
    
    simd_float2 v3 = simd_abs(a_v3);
    
    /*Now wthe additional multiplication
     
     Note that we're now in vector notation
     
     we want to take
     v0x,v0y,v2x,v2y,v4x,v4y
     
     x
     
     0.15,0.15,0.15,0.15,0.26,0.26
     
                                                      
     
     */
#ifndef BC_NO_VEC8
    simd_float4 v0_v4_for_mul = takenAbs_v0_v4_v2_v1.lo;
#else
    simd_float4 v0_v4_for_mul = takenAbs_v0_v4;
#endif
    simd_float4 v0_v4_coef = simd_make_float4(0.15,0.15,0.15,0.15);
    
    simd_float4 v0_v4_multiplied = v0_v4_for_mul * v0_v4_coef;
    
    //we are only multiplying by 6 terms so we don't care about the last 2
    simd_float2 v2_dc_coef = simd_make_float2(0.26666666666666666,0.26666666666666666);
#ifndef BC_NO_VEC8
    simd_float2 v2_unmultiplied = takenAbs_v0_v4_v2_v1.hi.lo;
#else
    simd_float2 v2_unmultiplied = takenAbs_v2_v1.lo;
#endif
    simd_float2 v2 = v2_unmultiplied * v2_dc_coef;
    
    /*To do our final reduction, we need to add v0,v1,v2,v3,v4 (5 terms) in 2 sets (seprately for x,y)
     
     implement
     
     simd4 x_0 = v0,v1,v2,v3
     simd4 y_0 = v0,v1,v2,v3
     reduce(x_0)
     reduce(y_0)
     
     x_0,y_0 + (v4)
     
     Trying the first I guess, it's illegal in metal but we could potentially compare with #2
     
     */
    
    //get xy splits for v0,v4
    simd_float2 x_v0_v4 = v0_v4_multiplied.even;
    simd_float2 y_v0_v4 = v0_v4_multiplied.odd;
    
    //get xy splits for v1,v3
    simd_float2 x_v1_v3 = simd_make_float2(v1.x, v3.x);
    simd_float2 y_v1_v3 = simd_make_float2(v1.y,v3.y);
    
    //xy splits for v2 are trivial
    
    //reduce above terms
    bc_float_t reduced_x_v0_v1_v3_v4 = simd_reduce_add(simd_make_float4(x_v0_v4, x_v1_v3));
    bc_float_t reduced_x = reduced_x_v0_v1_v3_v4 + v2.x;
    bc_float_t reduced_y_v0_v1_v3_v4 = simd_reduce_add(simd_make_float4(y_v0_v4, y_v1_v3));
    bc_float_t reduced_y = reduced_y_v0_v1_v3_v4 + v2.y;
    
    
    bc_float2_t reduce2D = simd_make_float2(reduced_x,reduced_y);
    return simd_length(reduce2D);
}

BCCubic2 BCCubicSplit(BCCubic c, bc_float_t t) {
    bc_float_t t_minus_1 = t - 1;
    bc_float2_t t_minus_1_d = t_minus_1 * c.d;
    bc_float2_t t_c = t * c.c;
    bc_float_t t_minus_1_squared = pow(t_minus_1, 2);
    bc_float2_t t_minus_1_squared_c = c.c * t_minus_1_squared;
    bc_float_t t_2 = 2 * t;
    
    float t_squared = pow(t,2);
    
    BCCubic2 out;
    out.a.xy = c.a;
    bc_float2_t t1 = pow(t,3) * c.b;
    bc_float2_t t2 = - 3 * t_squared * t_minus_1_d;
    bc_float2_t t3 = 3 * t * t_minus_1_squared_c;
    bc_float2_t t4 =  - pow(t_minus_1,3) * c.a;
    out.a.zw = pow(t,3) * c.b - 3 * t_squared * t_minus_1_d + 3 * t * t_minus_1_squared_c - pow(t_minus_1,3) * c.a;
    out.b.xy = out.a.zw;
    out.b.zw = c.b;
    
    out.c.xy = t_c - t_minus_1 * c.a;
    out.c.zw = t_squared * c.b  - t_2 * t_minus_1_d + t_minus_1_squared_c;
    
    out.d.xy = t_squared * c.d - t_2 * t_minus_1 * c.c + t_minus_1_squared * c.a;
    out.d.zw = t * c.b - t_minus_1_d;
    return out;
    
    
    
    //    simd_float2 p1 = curve.a;
    //    simd_float2 p2 = curve.c;
    //    simd_float2 p3 = curve.d;
    //    simd_float2 p4 = curve.b;
    //
    //    //p1: a
    //    //p2 t * p2 - (t - 1) * p1
    //    //p3 pow(t, 2) * p3 - 2 * t * (t - 1) * p2 + pow(t - 1,2) * p1
    //    //p4 pow(t,3) * p4 - 3 * pow(t,2) * (t - 1) * p3 + 3 * t * pow(t - 1,2) * p2 - pow(t - 1,3) * p1
    //    simd_float2 c1b_subexpression1 = pow(t,3) * p4;
    //    simd_float2 c1b_subexpression2 = -3 * pow(t,2) * (t - 1) * p3;
    //    simd_float2 c1b_subexpression3 = 3 * t * pow(t - 1,2) * p2;
    //    simd_float2 c1b_subexpression4 = -pow(t - 1,3) * p1;
    //    simd_float2 c1b = c1b_subexpression1 + c1b_subexpression2 + c1b_subexpression3 + c1b_subexpression4;
    //    simd_float2 c1c = t * p2 - (t - 1) * p1;
    //    simd_float2 c1d = pow(t, 2) * p3 - 2 * t * (t - 1) * p2 + pow(t - 1,2) * p1;
    //
    //    DoubleCubic out;
    //
    //    out.a.a = p1;
    //    out.a.b = c1b;
    //    out.a.c = c1c;
    //    out.a.d = c1d;
    //
    //    //p1: curve1.b
    //    //p2 pow(t, 2) * p4 - 2 * t * (t - 1) * p3 + pow(t - 1,2) * p2
    //    //p3 t * p4 - (t - 1) * p3
    //    //p4 p4
    //    out.b.a = c1b;
    //    out.b.b = curve.b;
    //    out.b.c = pow(t, 2) * p4 - 2 * t * (t - 1) * p3 + pow(t - 1,2) * p2;
    //    out.b.d = t * p4 - (t - 1) * p3;
    //    return out;
}
