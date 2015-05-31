#ifndef DCTSORTER_HPP
#define DCTSORTER_HPP

#include <vector>
#include <map>

#include "greyimage.hpp"
#include "block.hpp"
#include "shift.hpp"
#include "offset.hpp"

class DCTSorter {
public:
    DCTSorter();
    void setGrey( const GreyImage& grey );
    GreyImage getGrey() const;
    void work();

private:
    GreyImage mGrey;
    std::vector<Block> mBlocks;
    std::map<Shift, int> mShifts;
    std::map<Shift, Block> mOffsets;

    std::vector<Block> mOriginals;
    std::vector<Block> mCopies;

    int mMinimum;

    void readGreyToBlocks();
    void dctBlocks();
    void sortBlocks();
    void sortShifts();
    void findDuplicates();
};

#endif // DCTSORTER_HPP
