#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopWidget>

#include "sorterconnection.hpp"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow ),
    mConnection( new SorterConnection( this ) ) {

    ui->setupUi( this );
    setupRecentImagesMenu();

    connect( ui->widgetControl, &ControlWidget::signalRun, mConnection, &SorterConnection::slotRun, Qt::UniqueConnection );
    connect( ui->widgetControl, &ControlWidget::signalImage, ui->scrollArea, &ScrollArea::slotDrawImage, Qt::UniqueConnection );
    connect( mConnection, &SorterConnection::signalDone, ui->widgetControl, &ControlWidget::slotResults, Qt::QueuedConnection );
    connect( mConnection, &SorterConnection::signalReset, ui->widgetControl, &ControlWidget::slotReset, Qt::UniqueConnection );
    connect( mConnection, &SorterConnection::signalProgress, ui->widgetControl, &ControlWidget::slotProgress, Qt::QueuedConnection );

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
    std::tie( ok, filename ) = mFileDealer.getOpenFilename();

    if( ok ) {
        this->slotOpenImage( filename );
    }

    /* regain focus */
    this->activateWindow();
}

void MainWindow::slotOpenImage( QString filename ) {
    QImage image;

    if( !filename.isEmpty() && image.load( filename ) ) {

        if( image.format() != QImage::Format_ARGB32_Premultiplied ) {
            image = image.convertToFormat( QImage::Format_ARGB32_Premultiplied );
        }

        this->mConnection->setImage( image );
        ui->widgetControl->setImage( image );

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
    mFileDealer.setType( FileDealer::ImagesType );
    QMenu* recent = new QMenu( tr( "Recent" ), this->ui->menuFile );
    mFileDealer.setupRecentMenu( recent );

    connect( &mFileDealer, &FileDealer::signalOpen, this, &MainWindow::slotOpenImage, Qt::UniqueConnection );

    this->ui->menuFile->insertMenu( ui->actionExit, recent );
    this->ui->menuFile->insertSeparator( ui->actionExit );
}

void MainWindow::on_actionRun_triggered() {
    ui->widgetControl->on_buttonRun_clicked();
}
