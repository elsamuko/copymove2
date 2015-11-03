#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>

#include "ui_about.h"
#include "about.hpp"
#include "log/log.hpp"
#include "log/datetime.hpp"

About::About( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::About ) {

    ui->setupUi( this );

    /* no resize */
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );

    /* version info */
    QString message = tr( "Version: " )
                      + QString::number( COPYMOVE_VERSION_MAJOR ) + "."
                      + QString::number( COPYMOVE_VERSION_MINOR ) + "."
                      + QString::number( COPYMOVE_VERSION_RELEASE ) + "\n";
    message += QString( __DATE__ ) + " " + QString( __TIME__ );
    ui->labelVersion->setText( message );

    /* copyright information */
    ui->labelCopyright->setText( "\xC2\xA9 " + QString::number( datetime::compilationYear() )
                                 + "\nFD Imaging UG (haftungsbeschr\xC3\xA4nkt)" );

    /* logo */
    QPixmap pixmap( ":/icons/icons/logo.png" );
    ui->labelLogo->setPixmap( pixmap.scaledToWidth( ui->labelLogo->width(), Qt::SmoothTransformation ) );

    /* license info */
    int i = 0;
    QString dir = ":/licenses/licenses/";
    ui->boxLicense->insertItem( i++, "Fontconfig",  QVariant( dir + "fontconfig.txt" ) );
    ui->boxLicense->insertItem( i++, "ImageMagick", QVariant( dir + "imagemagick.txt" ) );
    ui->boxLicense->insertItem( i++, "lcms2",       QVariant( dir + "lcms2.txt" ) );
    ui->boxLicense->insertItem( i++, "libbz2",      QVariant( dir + "libbz2.txt" ) );
    ui->boxLicense->insertItem( i++, "libfreetype", QVariant( dir + "libfreetype.txt" ) );
    ui->boxLicense->insertItem( i++, "libiconv",    QVariant( dir + "libiconv.txt" ) );
    ui->boxLicense->insertItem( i++, "libjpeg",     QVariant( dir + "libjpeg.txt" ) );
    ui->boxLicense->insertItem( i++, "libltdl",     QVariant( dir + "libltdl.txt" ) );
    ui->boxLicense->insertItem( i++, "liblzma",     QVariant( dir + "liblzma.txt" ) );
    ui->boxLicense->insertItem( i++, "libopenjpeg", QVariant( dir + "libopenjpeg.txt" ) );
    ui->boxLicense->insertItem( i++, "libpng",      QVariant( dir + "libpng.txt" ) );
    ui->boxLicense->insertItem( i++, "libtiff",     QVariant( dir + "libtiff.txt" ) );
    ui->boxLicense->insertItem( i++, "libxml2",     QVariant( dir + "libxml2.txt" ) );
    ui->boxLicense->insertItem( i++, "libz",        QVariant( dir + "libz.txt" ) );
    ui->boxLicense->insertItem( i++, "Qt",          QVariant( dir + "qt.txt" ) );
    ui->boxLicense->insertItem( i++, "TCLAP",       QVariant( dir + "tclap.txt" ) );
}

About::~About() {
    delete ui;
}

void About::on_boxLicense_currentIndexChanged( int index ) {
    QString filename = ui->boxLicense->itemData( index ).toString();

    QFile file( filename );

    if( file.open( QFile::ReadOnly | QFile::Text ) ) {
        QTextStream stream( &file );
        ui->browserLicense->setPlainText( stream.readAll() );
    } else {
        qWarning() << "Could not open " + filename;
    }
}

void About::on_buttonWebsite_clicked() {
    QDesktopServices::openUrl( QUrl( "http://www.fd-imaging.com" ) );
}

void About::on_buttonFeedback_clicked() {
    QDesktopServices::openUrl( QUrl( "http://fd-imaging.com/apps-2/copymove/feedback/" ) );
}
