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
        void signalStop();
        void signalImage( QImage image, bool fit );
        void signalSendFirstBlock( PointI pos );
        void signalSendSecondBlock( PointI pos );

    public slots:
        void on_buttonRun_clicked();
        void slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end );
        void slotProgress( size_t progress );
        void slotSetFirstBlock( QPoint pos );
        void slotSetSecondBlock( QPoint pos );
        void slotReset();

    private slots:
        void on_comboHits_currentIndexChanged( int index );
        void on_spinBoxFirstX_valueChanged( int first );
        void on_spinBoxFirstY_valueChanged( int first );
        void on_spinBoxSecondX_valueChanged( int second );
        void on_spinBoxSecondY_valueChanged( int second );
        void on_checkBoxShowShift_stateChanged( int );
        void on_checkBoxShowBestHits_stateChanged( int );
        void on_checkBoxShowMedian_stateChanged( int );
        void on_checkBoxShowAverage_stateChanged( int );
        void on_buttonStop_clicked();

    private:
        void updateBlockStats();
        Ui::ControlWidget* ui;
        QImage mImage;
};
