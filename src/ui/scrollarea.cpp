#include "scrollarea.hpp"

#include <QWheelEvent>
#include <QLabel>

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

void ScrollArea::setImage( const QImage image ) {
    mLabel->setPixmap( QPixmap::fromImage( image ) );
    mLabel->adjustSize();

    float scaled   = this->size().width() - 5;
    float original = image.size().width();

    mZoom = scaled / original;
    this->zoom();
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

void ScrollArea::zoom() {
    mLabel->resize( mZoom * mLabel->pixmap()->size() );
    LOG( "Zoom : " + std::to_string( mZoom ) );
}
