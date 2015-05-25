#ifndef DCTSORTER_HPP
#define DCTSORTER_HPP

#include <vector>
#include <map>

#include "greyimage.hpp"
#include "block.hpp"
#include "shift.hpp"

class DCTSorter {
public:
    DCTSorter();
    void setGrey( const GreyImage& grey );
    GreyImage getGrey() const;
    void work();

private:
    GreyImage mGrey;
    std::vector<Block> mBlocks;
    std::map<Shift, int> mShiftCount;

    void readGreyToBlocks();
    void dctBlocks();
    void sortBlocks();
    void findDuplicates();
};

#endif // DCTSORTER_HPP
