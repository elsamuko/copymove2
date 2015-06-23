#pragma once

#include <vector>
#include <list>
#include <map>

#include "greyimage.hpp"
#include "block.hpp"
#include "shift.hpp"
#include "offset.hpp"
#include "threadpool.hpp"
#include "shifthit.hpp"

class DCTSorter {
    public:
        struct ShiftImages {
            GreyImage from;
            GreyImage to;
        };
        DCTSorter( size_t minHits );
        void setGrey( const GreyImage& grey );
        GreyImage getGrey() const;
        DCTSorter::ShiftImages getShiftImages() const;
        std::vector<ShiftHit> getShiftHits() const;
        void work();

    private:
        ThreadPool mThreadPool;
        GreyImage mGrey;
        ShiftImages mResult;
        std::vector<Block> mBlocks;
        std::map<Shift, std::list<std::pair<Block, Block>>> mShifts;
        std::vector<ShiftHit> mShiftHits;

        PointI mImageSize;

        std::vector<Block> mOriginals;
        std::vector<Block> mCopies;

        size_t mMinHits;

        void readGreyToBlocks();
        void dctBlocks();
        void sortBlocks();
        void sortShifts();
        void findDuplicates();

        void debugBlocks();
};
