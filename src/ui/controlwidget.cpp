#include "controlwidget.hpp"
#include "ui_controlwidget.h"

#include <QPainter>

#include "block.hpp"

Q_DECLARE_METATYPE( ShiftHit )

ControlWidget::ControlWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::ControlWidget ) {
    ui->setupUi( this );

    ui->buttonRun->show();
    ui->progressBar->hide();
    ui->progressBar->reset();

    ui->tabBlocks->setDisabled( true );
    ui->tabHits->setDisabled( true );
    ui->tabAnalysis->setDisabled( true );
}

ControlWidget::~ControlWidget() {
    delete ui;
}

void ControlWidget::setImage( QImage image ) {

    QSize size = image.size();

    if( size.isEmpty() || image.isNull() ) {
        LOG_WARNING( "Empty image received" );
        return;
    }

    ui->tabBlocks->setEnabled( true );
    ui->tabAnalysis->setEnabled( true );
    mImage = image;
    signalImage( image, true );

    // positions
    ui->spinBoxFirstX->setMaximum( size.width() - Block::size );
    ui->spinBoxFirstY->setMaximum( size.height() - Block::size );
    ui->spinBoxSecondX->setMaximum( size.width() - Block::size );
    ui->spinBoxSecondY->setMaximum( size.height() - Block::size );

    // shift
    ui->spinBoxComparedDX->setMaximum( size.width() - Block::size );
    ui->spinBoxComparedDY->setMaximum( size.height() - Block::size );
    ui->spinBoxComparedDX->setMinimum( Block::size - size.width() );
    ui->spinBoxComparedDY->setMinimum( Block::size - size.height() );

    // stats
    updateBlockStats();
}

void ControlWidget::slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end ) {

    ui->buttonRun->show();
    ui->progressBar->hide();
    ui->progressBar->reset();
    ui->tabHits->setEnabled( true );

    ui->comboHits->clear();

    for( auto it = begin; it != end; ++it ) {
        QString ranking = QString::number( it->ranking() ) + " : " + QString::number( it->hits() );
        QVariant data;
        data.setValue<ShiftHit>( *it );
        ui->comboHits->addItem( ranking, data );
    }
}

void ControlWidget::slotProgress( size_t progress ) {
    ui->progressBar->setValue( progress );
}

void ControlWidget::slotReset() {
    ui->buttonRun->show();
    ui->progressBar->hide();
}

void ControlWidget::on_comboHits_currentIndexChanged( int ) {
    ShiftHit hit = ui->comboHits->currentData().value<ShiftHit>();
    ui->textHit->setText( QString::fromStdString( hit.toString() ) );

    QImage copy  = mImage.scaled( mImage.size() );
    QImage overlay( mImage.size(), QImage::Format_ARGB32_Premultiplied );
    overlay.fill( 0 );

    {
        QPainter painter( &overlay );
        // painter.setRenderHint( QPainter::Antialiasing, true );

        std::list<std::pair<PointI, PointI>>& pairs = hit.getBlocks();

        QBrush reddish = QBrush( QColor( 235, 141,  89, 255 ) );
        QBrush bluish  = QBrush( QColor( 145, 191, 255, 255 ) );

        for( std::pair<PointI, PointI>& pair : pairs ) {
            painter.fillRect( QRect( pair.first.x(), pair.first.y(), Block::size, Block::size ), bluish );
            painter.fillRect( QRect( pair.second.x(), pair.second.y(), Block::size, Block::size ), reddish );
        }
    }

    {
        QPainter painter( &copy );
        painter.setOpacity( 0.5 );
        painter.setCompositionMode( QPainter::CompositionMode_ColorDodge );
        painter.drawImage( 0, 0, overlay );

        painter.setOpacity( 1.f );
        painter.setCompositionMode( QPainter::CompositionMode_Source );
        painter.setPen( QPen( Qt::black, 3 ) );
        painter.drawLine( hit.x(), hit.y(), hit.x() + hit.dx(), hit.y() + hit.dy() );
        painter.setPen( QPen( Qt::green, 2 ) );
        painter.drawLine( hit.x(), hit.y(), hit.x() + hit.dx(), hit.y() + hit.dy() );
    }

    LOG( "Sending hit " + hit.toString() );
    emit signalImage( copy, false );
}

void ControlWidget::on_buttonRun_clicked() {
    SorterParams params;
    params.setMinimalHits( ui->spinBoxMinHits->value() );
    params.setQuality( ui->spinBoxQuality->value() );
    params.setValid( true );
    ui->buttonRun->hide();
    ui->progressBar->show();
    emit signalRun( params );
}

void ControlWidget::on_spinBoxFirstX_valueChanged( int first ) {
    int second = ui->spinBoxSecondX->value();
    ui->spinBoxComparedDX->setValue( first - second );
    updateBlockStats();
}

void ControlWidget::on_spinBoxFirstY_valueChanged( int first ) {
    int second = ui->spinBoxSecondY->value();
    ui->spinBoxComparedDY->setValue( first - second );
    updateBlockStats();
}

void ControlWidget::on_spinBoxSecondX_valueChanged( int second ) {
    int first = ui->spinBoxFirstX->value();
    ui->spinBoxComparedDX->setValue( first - second );
    updateBlockStats();
}

void ControlWidget::on_spinBoxSecondY_valueChanged( int second ) {
    int first = ui->spinBoxFirstY->value();
    ui->spinBoxComparedDY->setValue( first - second );
    updateBlockStats();
}

void ControlWidget::updateBlockStats() {

    const uchar* bits   = mImage.constBits();
    QSize size =  mImage.size();

    Block first;
    first.setPos( PointI( ui->spinBoxFirstX->value(), ui->spinBoxFirstY->value() ) );

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            int pos = size.width() * ( first.y() + y ) + ( first.x() + x );
            first[x][y] = bits[pos + 1]; // green
        }
    }

    first.calculateStandardDeviation();
    first.dct();

    Block second;
    second.setPos( PointI( ui->spinBoxSecondX->value(), ui->spinBoxSecondY->value() ) );

    for( int y = 0; y < Block::size; ++y ) {
        for( int x = 0; x < Block::size; ++x ) {
            int pos = size.width() * ( second.y() + y ) + ( second.x() + x );
            second[x][y] = bits[pos + 1]; // green
        }
    }

    second.calculateStandardDeviation();
    second.dct();

    ui->lineEditFirst->setText( QString::fromStdString( first.toString() ) );
    ui->lineEditSecond->setText( QString::fromStdString( second.toString() ) );
    float similarity = first.cosineSimilarity( second );
    ui->labelSimilarityValue->setText( QString::number( similarity ) );

    QImage previewFirst = mImage.copy( first.x(), first.y(), Block::size, Block::size ).scaledToHeight( 48, Qt::FastTransformation );
    ui->labelFirstPreview->setPixmap( QPixmap::fromImage( previewFirst ) );

    QImage previewSecond = mImage.copy( second.x(), second.y(), Block::size, Block::size ).scaledToHeight( 48, Qt::FastTransformation );
    ui->labelSecondPreview->setPixmap( QPixmap::fromImage( previewSecond ) );

    if( similarity < 0.9 ) {
        // red
        ui->labelSimilarityValue->setStyleSheet( "QLabel { background-color : hsl(0, 50%, 100%); }" );
    } else {
        // red -> yellow -> green
        int hue = ( similarity - 0.9 ) * 1200;
        int sat = 50 + ( similarity - 0.9 ) * 500;
        ui->labelSimilarityValue->setStyleSheet( "QLabel { background-color : hsl(" + QString::number( hue ) + ", " + QString::number( sat ) + "%, 100%); }" );
    }
}
