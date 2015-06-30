#pragma once

#include <QFuture>
#include <QObject>
#include <QImage>

#include "dctsorter.hpp"
#include "sorterparams.hpp"

Q_DECLARE_METATYPE( std::vector<ShiftHit>::const_iterator )

class SorterConnection : public QObject {
        Q_OBJECT
    public:
        explicit SorterConnection( QObject* parent = 0 );
        void setImage( QImage image );

    signals:
        void signalDone( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end );
        void signalProgress( size_t );
        void signalReset();

    public slots:
        void slotRun( SorterParams params );

    private:
        GreyImage getGrey() const;

        QImage mImage;
        DCTSorter mSorter;
        std::vector<ShiftHit> mShiftHits;
        QFuture<void> mWhenFinished;
};
