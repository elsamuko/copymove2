#pragma once

#include <QWidget>

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

    signals:
        void signalRun( SorterParams params );
        void signalHit( QImage image );

    public slots:
        void slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end );

    private slots:
        void on_pushButton_clicked();
        void on_comboHits_currentIndexChanged( int );

    private:
        Ui::ControlWidget* ui;
};
