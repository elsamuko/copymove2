/*
Copyright
    Copyright(C) 1997,2001 Takuya OOURA (email: ooura@kurims.kyoto-u.ac.jp).
    You may use, copy, modify this code for any purpose and
    without fee. You may distribute this ORIGINAL package.
*/


#include <vector>

namespace ooura {
    void ddct8x8s( int isgn, std::vector<std::vector<double>>& a );
    void ddct16x16s( int isgn, std::vector<std::vector<double>>& a );
}
