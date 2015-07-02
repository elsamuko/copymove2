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
        void on_spinBoxFirstX_valueChanged( int first );
        void on_spinBoxFirstY_valueChanged( int first );
        void on_spinBoxSecondX_valueChanged( int second );
        void on_spinBoxSecondY_valueChanged( int second );

    private:
        void updateBlockStats();
        Ui::ControlWidget* ui;
        QImage mImage;
};
