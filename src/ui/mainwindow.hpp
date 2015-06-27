#pragma once

#include <QMainWindow>
#include <QImage>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();

    private slots:
        void on_actionExit_triggered();
        void on_actionOpen_triggered();

    private:
        Ui::MainWindow* ui;

        QImage mImage;
};
