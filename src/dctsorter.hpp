#pragma once

#include <vector>
#include <list>
#include <map>
#include <functional>

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
        ~DCTSorter();
        enum State { Idling, Running, Stopping };
        void setGrey( const GreyImage& grey );
        GreyImage getGrey() const;
        DCTSorter::ShiftImages getShiftImages() const;
        std::vector<ShiftHit> getShiftHits() const;
        void setParams( const SorterParams& params );
        void reset( bool hard = true );
        void work();
        void start();
        void stop();
        void resume();
        bool stopped() const;
        void setProgressCallback( const std::function<void( size_t )>& callback );

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
        std::mutex mStateLock;
        DCTSorter::State mState;

        bool mGreyReceived;
        bool mWorked;
        bool mBlocksSet;
        bool mDCTCalculated;
        bool mBlocksSorted;
        bool mDuplicatesSearched;
        bool mShiftsSorted;

        std::function<void( size_t )> mProgressCallback;

        void readGreyToBlocks();
        void sortBlocks();
        void sortShifts();
        void searchDuplicates();
        void debugBlocks();
        void setProgress( size_t percentage );
};
