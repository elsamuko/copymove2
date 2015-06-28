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
    connect( mConnection, &SorterConnection::signalDone, ui->widgetControl, &ControlWidget::slotResults, Qt::UniqueConnection );

    // center app
    // \sa https://wiki.qt.io/Center_a_Window_on_the_Screen
    this->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry() ) );
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
    if( !filename.isEmpty() && mImage.load( filename ) ) {
        ui->scrollArea->setImage( mImage );
        this->mConnection->setImage( mImage );
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
