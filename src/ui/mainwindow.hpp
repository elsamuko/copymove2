#pragma once

#include <QMainWindow>
#include <QImage>

#include "greyimage.hpp"
#include "ui/filedealer.hpp"

class SorterConnection;

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
        void slotOpenImage( QString filename );

    private:
        void setupRecentImagesMenu();

        Ui::MainWindow* ui;

        SorterConnection* mConnection;

        /* io helper class */
        FileDealer mFileDealer;
};
