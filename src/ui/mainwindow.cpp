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
    QString title   = tr( "Open image" );
    QString filter  = tr( "Images (*.jpg *.jpeg *.JPG *.JPEG *.png *.PNG)" );
    QString desktop = QStandardPaths::writableLocation( QStandardPaths::DesktopLocation );

    QString filename = QFileDialog::getOpenFileName( 0, title, desktop, filter );

    if( !filename.isEmpty() && mImage.load( filename ) ) {
        ui->scrollArea->setImage( mImage );
        this->mConnection->setImage( mImage );
    }
}
