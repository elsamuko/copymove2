#pragma once

#include <vector>
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
    DCTSorter();
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
    std::map<Shift, std::vector<std::pair<Block,Block>>> mShifts;
    std::vector<ShiftHit> mShiftHits;

    int mWidth;
    int mHeight;

    std::vector<Block> mOriginals;
    std::vector<Block> mCopies;

    size_t mMaxHits;

    void readGreyToBlocks();
    void dctBlocks();
    void sortBlocks();
    void sortShifts();
    void findDuplicates();

    void debugBlocks();
};
