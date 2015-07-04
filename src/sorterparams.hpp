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

        size_t quality() const {
            return mQuality;
        }
        void setQuality( const size_t quality ) {
            mQuality = quality;
        }

        std::string filename() const {
            return mFilename;
        }
        void setFilename( const std::string& filename ) {
            mFilename = filename;
        }

        friend std::ostream& operator <<( std::ostream& stream, const SorterParams& p );
        std::string toString() const;

    private:
        bool mValidParams;
        size_t mMinimalHits;
        size_t mQuality;
        std::string mFilename;
};
