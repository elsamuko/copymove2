#include "controlwidget.hpp"
#include "ui_controlwidget.h"

#include <QPainter>

Q_DECLARE_METATYPE( ShiftHit )

ControlWidget::ControlWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::ControlWidget ) {
    ui->setupUi( this );

    ui->buttonRun->show();
    ui->progressBar->hide();
    ui->progressBar->reset();
}

ControlWidget::~ControlWidget() {
    delete ui;
}

void ControlWidget::slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end ) {

    ui->buttonRun->show();
    ui->progressBar->hide();
    ui->progressBar->reset();

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

    QImage image( hit.imageSize().x(), hit.imageSize().y(), QImage::Format_ARGB32_Premultiplied );
    image.fill( 0 );

    QPainter painter( &image );
    // painter.setRenderHint( QPainter::Antialiasing, true );

    std::list<std::pair<PointI, PointI>>& pairs = hit.getBlocks();

    QBrush reddish = QBrush( QColor( 235, 141,  89, 255 ) );
    QBrush bluish  = QBrush( QColor( 145, 191, 255, 255 ) );

    for( std::pair<PointI, PointI>& pair : pairs ) {
        painter.fillRect( QRect( pair.first.x(), pair.first.y(), Block::size, Block::size ), bluish );
        painter.fillRect( QRect( pair.second.x(), pair.second.y(), Block::size, Block::size ), reddish );
    }

    LOG( "Sending hit " + hit.toString() );
    emit signalHit( image );
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
