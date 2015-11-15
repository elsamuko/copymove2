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
        void slotExportImage( QString filename );
        void slotExportData( QString filename );
        void slotDone();
        void on_actionRun_triggered();
        void on_actionAbout_triggered();
        void on_actionActualPixels_triggered();
        void on_actionShowLogfile_triggered();
        void on_actionZoomIn_triggered();
        void on_actionZoomOut_triggered();
        void on_actionExportImage_triggered();
        void on_actionExportData_triggered();

    private:
        void scaleImage( QImage& image );
        void setupRecentImagesMenu();

        Ui::MainWindow* ui;

        SorterConnection* mConnection;

        /* io helper class */
        FileDealer* mImageDealer;
        FileDealer* mDataDealer;

        /* drag & drop */
        virtual void dropEvent( QDropEvent* event );
        virtual void dragEnterEvent( QDragEnterEvent* event );
};
