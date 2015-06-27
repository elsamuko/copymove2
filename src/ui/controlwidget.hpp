#pragma once

#include <QWidget>

#include "sorterparams.hpp"

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

    private slots:
        void on_pushButton_clicked();

    private:
        Ui::ControlWidget* ui;
};
