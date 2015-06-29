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
#include "sorterparams.hpp"

class DCTSorter {
    public:
        struct ShiftImages {
            GreyImage from;
            GreyImage to;
        };
        DCTSorter();
        void setGrey( const GreyImage& grey );
        GreyImage getGrey() const;
        DCTSorter::ShiftImages getShiftImages() const;
        std::vector<ShiftHit> getShiftHits() const;
        void setParams( const SorterParams& params );
        void work();

    private:
        ThreadPool mThreadPool;
        GreyImage mGrey;
        ShiftImages mResult;
        std::vector<Block> mBlocks;
        std::map<Shift, std::list<std::pair<PointI, PointI>>> mShifts;
        std::vector<ShiftHit> mShiftHits;

        PointI mImageSize;
        SorterParams mParams;

        // state checks
        bool mGreyReceived;
        bool mWorked;
        bool mBlocksSet;
        bool mDCTCalculated;
        bool mBlocksSorted;
        bool mDuplicatesSearched;
        bool mShiftsSorted;

        void readGreyToBlocks();
        void dctBlocks();
        void sortBlocks();
        void sortShifts();
        void searchDuplicates();
        void debugBlocks();
        void reset();
};
