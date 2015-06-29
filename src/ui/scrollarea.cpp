#include "scrollarea.hpp"

#include <QWheelEvent>
#include <QLabel>
#include <QPainter>

#include "log/log.hpp"

ScrollArea::ScrollArea( QWidget* parent ) :
    QScrollArea( parent ),
    mZoom( 1.f ) {

    this->setBackgroundRole( QPalette::Shadow );

    mLabel = new QLabel( this );
    mLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    mLabel->setScaledContents( true );
    this->setWidget( mLabel );
    this->setWidgetResizable( true );
}

void ScrollArea::autoZoom() {

    if( mImage.isNull() ) {
        return;
    }

    float scaledW   = this->size().width();
    float originalW = mImage.size().width();

    float scaledH   = this->size().height();
    float originalH = mImage.size().height();

    mZoom = std::min( scaledW / originalW, scaledH / originalH ) * 0.97;
    this->zoom();
}

void ScrollArea::setImage( const QImage image ) {
    Q_ASSERT( !image.isNull() );

    if( image.format() != QImage::Format_ARGB32_Premultiplied ) {
        mImage = image.convertToFormat( QImage::Format_ARGB32_Premultiplied );
    } else {
        mImage = image;
    }

    mLabel->setPixmap( QPixmap::fromImage( mImage ) );
    mLabel->adjustSize();

    autoZoom();
}

void ScrollArea::slotDrawOverlay( QImage overlay ) {
    Q_ASSERT( overlay.format() == QImage::Format_ARGB32_Premultiplied );
    Q_ASSERT( overlay.size() == mImage.size() );

    QPixmap pixmap = QPixmap::fromImage( mImage );
    QPainter painter( &pixmap );
    painter.setOpacity( 0.5 );
    painter.setCompositionMode( QPainter::CompositionMode_ColorDodge );
    painter.drawImage( 0, 0, overlay );
    mLabel->setPixmap( pixmap );
}

void ScrollArea::wheelEvent( QWheelEvent* event ) {

    if( mLabel->pixmap() ) {

        this->setWidgetResizable( false );
        float delta = event->delta();

        // clip range from -1..1
        delta /= 120.f;
        delta = std::min( std::max( delta, -1.f ), 1.f );

        mZoom += mZoom * delta / 25.f;
        this->zoom();
    }
}

void ScrollArea::mouseDoubleClickEvent( QMouseEvent* ) {
    this->autoZoom();
}

void ScrollArea::zoom() {
    mLabel->resize( mZoom * mLabel->pixmap()->size() );
    LOG( "Zoom : " + std::to_string( mZoom ) );
}
