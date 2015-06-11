/*
Copyright
    Copyright(C) 1997,2001 Takuya OOURA (email: ooura@kurims.kyoto-u.ac.jp).
    You may use, copy, modify this code for any purpose and
    without fee. You may distribute this ORIGINAL package.
*/


#include <vector>

namespace ooura {

    template<int i>
    void ddct( int, std::vector<std::vector<double>>& );

    template<>
    void ddct<8>( int isgn, std::vector<std::vector<double>>& a );

    template<>
    void ddct<16>( int isgn, std::vector<std::vector<double>>& a );
}
