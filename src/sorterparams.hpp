#pragma once

#include <cstdlib>
#include <string>

class SorterParams {
    public:
        SorterParams();

        bool valid() const {
            return mValidParams;
        }
        void setValid( const bool valid ) {
            mValidParams = valid;
        }

        size_t minimalHits() const {
            return mMinimalHits;
        }
        void setMinimalHits( const size_t hits ) {
            mMinimalHits = hits;
        }

        std::string filename() const {
            return mFilename;
        }
        void setFilename( const std::string& filename ) {
            mFilename = filename;
        }

    private:
        bool mValidParams;
        size_t mMinimalHits;
        std::string mFilename;
};
