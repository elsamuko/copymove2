#pragma once

#include <QWidget>
#include <QImage>

#include "sorterparams.hpp"
#include "shifthit.hpp"

namespace Ui {
    class ControlWidget;
}

class ControlWidget : public QWidget {
        Q_OBJECT

    public:
        explicit ControlWidget( QWidget* parent = 0 );
        ~ControlWidget();
        void setImage( QImage image );

    signals:
        void signalRun( SorterParams params );
        void signalImage( QImage image, bool fit );

    public slots:
        void slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end );
        void slotProgress( size_t progress );
        void slotReset();

    private slots:
        void on_buttonRun_clicked();
        void on_comboHits_currentIndexChanged( int );

    private:
        Ui::ControlWidget* ui;
        QImage mImage;
};
