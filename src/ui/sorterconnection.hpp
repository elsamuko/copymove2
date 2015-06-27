#pragma once

#include <QObject>
#include <QImage>

#include "dctsorter.hpp"
#include "sorterparams.hpp"

class SorterConnection : public QObject {
        Q_OBJECT
    public:
        explicit SorterConnection( QObject* parent = 0 );

    signals:

    public slots:
        void slotRun( SorterParams params );

    private:
        GreyImage getGrey() const;

        QImage mImage;
        DCTSorter mSorter;
};
