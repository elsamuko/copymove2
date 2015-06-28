#include "controlwidget.hpp"
#include "ui_controlwidget.h"

ControlWidget::ControlWidget( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::ControlWidget ) {
    ui->setupUi( this );
}

ControlWidget::~ControlWidget() {
    delete ui;
}

void ControlWidget::slotResults( std::vector<ShiftHit>::const_iterator begin, std::vector<ShiftHit>::const_iterator end ) {

    ui->comboHits->clear();

    for( auto it = begin; it != end; ++it ) {
        QString ranking = QString::number( it->ranking() );
        QVariant data( QString::fromStdString( it->toString() ) );
        ui->comboHits->addItem( ranking, data );
    }
}

void ControlWidget::on_pushButton_clicked() {
    SorterParams params;
    params.mMinimalHits = ui->spinBoxMinHits->value();
    emit signalRun( params );
}

void ControlWidget::on_comboHits_currentIndexChanged( int ) {
    QString data = ui->comboHits->currentData().toString();
    ui->textHit->setText( data );
}
