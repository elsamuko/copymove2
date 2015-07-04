#include <sstream>

#include "sorterparams.hpp"


SorterParams::SorterParams() :
    mValidParams( false ),
    mMinimalHits( 10 ),
    mQuality( 5 ) {
}

std::ostream& operator <<( std::ostream& stream, const SorterParams& p ) {

    if( p.mValidParams ) {
        stream << "[X]";
    } else {
        stream << "[ ]";
    }

    stream << " M:" << p.mMinimalHits;
    stream << " Q:" << p.mQuality;

    if( !p.mFilename.empty() ) {
        stream << " File: " << p.mFilename;
    }

    return stream;
}

std::string SorterParams::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}
