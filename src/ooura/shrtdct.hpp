/*
Short Discrete Cosine Transform
    data length :16x16
    method      :row-column, radix 4 FFT
functions
    ddct16x16s: 16x16 DCT
function prototypes
    void ddct16x16s(int isgn, double **a);

Copyright
    Copyright(C) 1997,2001 Takuya OOURA (email: ooura@kurims.kyoto-u.ac.jp).
    You may use, copy, modify this code for any purpose and
    without fee. You may distribute this ORIGINAL package.
*/


/*
-------- 16x16 DCT (Discrete Cosine Transform) / Inverse of DCT --------
    [definition]
        <case1> Normalized 16x16 IDCT
            C[k1][k2] = (1/8) * sum_j1=0^15 sum_j2=0^15
                            a[j1][j2] * s[j1] * s[j2] *
                            cos(pi*j1*(k1+1/2)/16) *
                            cos(pi*j2*(k2+1/2)/16), 0<=k1<16, 0<=k2<16
                            (s[0] = 1/sqrt(2), s[j] = 1, j > 0)
        <case2> Normalized 16x16 DCT
            C[k1][k2] = (1/8) * s[k1] * s[k2] * sum_j1=0^15 sum_j2=0^15
                            a[j1][j2] *
                            cos(pi*(j1+1/2)*k1/16) *
                            cos(pi*(j2+1/2)*k2/16), 0<=k1<16, 0<=k2<16
                            (s[0] = 1/sqrt(2), s[j] = 1, j > 0)
    [usage]
        <case1>
            ddct16x16s(1, a);
        <case2>
            ddct16x16s(-1, a);
    [parameters]
        a[0...15][0...15] :input/output data (double **)
                           output data
                               a[k1][k2] = C[k1][k2], 0<=k1<16, 0<=k2<16
*/

namespace ooura {
    void ddct16x16s( int isgn, double** a );
}
