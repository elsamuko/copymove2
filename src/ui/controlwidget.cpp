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

void ControlWidget::on_pushButton_clicked() {
    SorterParams params;
    params.mMinimalHits = ui->spinBoxMinHits->value();
    emit signalRun( params );
}
