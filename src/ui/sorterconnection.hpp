#pragma once

#include <QObject>
#include <QImage>

#include "dctsorter.hpp"
#include "sorterparams.hpp"

class SorterConnection : public QObject {
        Q_OBJECT
    public:
        explicit SorterConnection( QObject* parent = 0 );
        void setImage( QImage image );

    signals:
        void signalDone( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end );

    public slots:
        void slotRun( SorterParams params );

    private:
        GreyImage getGrey() const;

        QImage mImage;
        DCTSorter mSorter;
        std::vector<ShiftHit> mShiftHits;
};
