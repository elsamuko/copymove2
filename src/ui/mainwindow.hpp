#pragma once

#include <QMainWindow>
#include <QImage>

class SorterConnection;
class FileDealer;

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
        void on_actionRun_triggered();
        void on_actionAbout_triggered();
        void on_actionActual_Pixels_triggered();

    private:
        void scaleImage( QImage& image );
        void setupRecentImagesMenu();

        Ui::MainWindow* ui;

        SorterConnection* mConnection;

        /* io helper class */
        FileDealer* mFileDealer;

        /* drag & drop */
        virtual void dropEvent( QDropEvent* event );
        virtual void dragEnterEvent( QDragEnterEvent* event );
};
