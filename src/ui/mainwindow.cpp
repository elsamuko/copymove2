#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDesktopWidget>
#include <QMimeData>

#include "sorterconnection.hpp"
#include "ui/filedealer.hpp"
#include "ui/about.hpp"

// MegaPixel literal
constexpr unsigned long long int operator "" _MP( unsigned long long int in ) {
    return in * 1E6;
}

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow ),
    mConnection( new SorterConnection( this ) ),
    mFileDealer( new FileDealer( this ) ) {

    ui->setupUi( this );
    setupRecentImagesMenu();

    CHECK_QT_CONNECT( connect( ui->widgetControl, &ControlWidget::signalRun, mConnection, &SorterConnection::slotRun, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( ui->widgetControl, &ControlWidget::signalStop, mConnection, &SorterConnection::slotStop, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( ui->widgetControl, &ControlWidget::signalImage, ui->scrollArea, &ScrollArea::slotDrawImage, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( mConnection, &SorterConnection::signalDone, ui->widgetControl, &ControlWidget::slotResults, Qt::QueuedConnection ) );
    CHECK_QT_CONNECT( connect( mConnection, &SorterConnection::signalReset, ui->widgetControl, &ControlWidget::slotReset, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( mConnection, &SorterConnection::signalProgress, ui->widgetControl, &ControlWidget::slotProgress, Qt::QueuedConnection ) );

    // send block pos from scrollarea to control widget...
    CHECK_QT_CONNECT( connect( ui->scrollArea, &ScrollArea::signalSendFirstBlock, ui->widgetControl, &ControlWidget::slotSetFirstBlock, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( ui->scrollArea, &ScrollArea::signalSendSecondBlock, ui->widgetControl, &ControlWidget::slotSetSecondBlock, Qt::UniqueConnection ) );

    // ... and back
    CHECK_QT_CONNECT( connect( ui->widgetControl, &ControlWidget::signalSendFirstBlock, ui->scrollArea, &ScrollArea::slotSetFirstBlock, Qt::UniqueConnection ) );
    CHECK_QT_CONNECT( connect( ui->widgetControl, &ControlWidget::signalSendSecondBlock, ui->scrollArea, &ScrollArea::slotSetSecondBlock, Qt::UniqueConnection ) );

    // open image with double blick
    CHECK_QT_CONNECT( connect( ui->scrollArea, &ScrollArea::signalOpenImage, ui->actionOpen, &QAction::trigger, Qt::UniqueConnection ) );


    // center app
    // \sa https://wiki.qt.io/Center_a_Window_on_the_Screen
    this->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry() ) );

    ui->actionRun->setDisabled( true );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    qApp->quit();
}

void MainWindow::on_actionOpen_triggered() {

    QString filename;
    bool ok;
    std::tie( ok, filename ) = mFileDealer->getOpenFilename();

    if( ok ) {
        this->slotOpenImage( filename );
    }

    /* regain focus */
    this->activateWindow();
}

void MainWindow::scaleImage( QImage& image ) {
    size_t megapixels = image.width() * image.height();

    if( megapixels > 3_MP ) {
        LOG( "Image is bigger than 3 MP, resizing..." );

        float ratio = ( ( float )3_MP ) / megapixels;

        int newHeight = image.height() * std::sqrt( ratio );
        int newWidth  = image.width()  * std::sqrt( ratio );

        image = image.scaled( newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        qDebug() << "Image is now" << image.width() << "x" << image.height();
    }
}

void MainWindow::slotOpenImage( QString filename ) {
    QImage image;

    if( !filename.isEmpty() && image.load( filename ) ) {

        if( image.format() != QImage::Format_ARGB32_Premultiplied ) {
            image = image.convertToFormat( QImage::Format_ARGB32_Premultiplied );
        }

        qDebug() << "Open " << filename;

        // resize if bigger than 3 MP
        scaleImage( image );

        ui->widgetControl->setImage( image );
        this->mConnection->setImage( image );
        ui->actionRun->setEnabled( true );

        // set window title
        QFileInfo info( filename );
        QString windowTitle = "CopyMove2 - ";
        windowTitle += info.fileName() + " ";
        windowTitle += QString::number( image.width() )  + "x";
        windowTitle += QString::number( image.height() );
        this->setWindowTitle( windowTitle );
    }
}

void MainWindow::setupRecentImagesMenu() {
    mFileDealer->setType( FileDealer::ImagesType );
    QMenu* recent = new QMenu( tr( "Recent" ), this->ui->menuFile );
    mFileDealer->setupRecentMenu( recent );

    connect( mFileDealer, &FileDealer::signalOpen, this, &MainWindow::slotOpenImage, Qt::UniqueConnection );

    this->ui->menuFile->insertMenu( ui->actionExit, recent );
    this->ui->menuFile->insertSeparator( ui->actionExit );
}

void MainWindow::on_actionRun_triggered() {
    ui->widgetControl->on_buttonRun_clicked();
}

void MainWindow::on_actionAbout_triggered() {
    About about;
    about.exec();
}

void MainWindow::on_actionActual_Pixels_triggered() {
    ui->scrollArea->slotZoomActualPixels();
}

void MainWindow::dragEnterEvent( QDragEnterEvent* event ) {
    event->accept();
}

//! \see mainwindow.ui: acceptDrops
void MainWindow::dropEvent( QDropEvent* event ) {
    const QMimeData* data = event->mimeData();

    if( data->hasUrls() ) {
        QList<QUrl> urls = data->urls();

        if( !urls.empty() ) {
            /* open first image */
            QString filename = urls[0].path();

#ifdef WIN32

            // avoid "/C:/image.jpg"
            if( filename.startsWith( '/' ) ) {
                filename.remove( 0, 1 );
            }

#endif // WIN32

            QFileInfo info( filename );

            if( info.exists() && mFileDealer->knows( info.suffix() ) ) {
                mFileDealer->setFilename( filename );
                slotOpenImage( filename );
            } else {
                qWarning() << "Cannot open " << filename;
                ui->statusBar->showMessage( "Invalid file type", 2000 );
            }
        }
    }
}

void MainWindow::on_actionShow_logfile_triggered() {
    QDesktopServices::openUrl( QUrl( "file://" + QString::fromStdString( logging::logFilename() ) ) );
}
