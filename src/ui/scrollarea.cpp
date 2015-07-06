#include "scrollarea.hpp"

#include <QWheelEvent>
#include <QLabel>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QAction>

#include "log/log.hpp"
#include "block.hpp"

ScrollArea::ScrollArea( QWidget* parent ) :
    QScrollArea( parent ),
    mZoom( 1.f ) {

    this->setBackgroundRole( QPalette::Shadow );

    mLabel = new QLabel( this );
    mLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    mLabel->setScaledContents( true );
    this->setWidget( mLabel );
    this->setWidgetResizable( true );

    CHECK_QT_CONNECT( connect( this, &ScrollArea::customContextMenuRequested, this, &ScrollArea::contextMenu, Qt::UniqueConnection ) );
}

void ScrollArea::autoZoom() {

    if( mImage.size().isEmpty() ) {
        return;
    }

    float scaledW   = this->size().width();
    float originalW = mImage.size().width();

    float scaledH   = this->size().height();
    float originalH = mImage.size().height();

    mZoom = std::min( scaledW / originalW, scaledH / originalH ) * 0.99;
    this->zoom();
}

void ScrollArea::scrollBy( const QPointF& diff ) {
    horizontalScrollBar()->setValue( horizontalScrollBar()->value() + diff.x() );
    verticalScrollBar()->setValue( verticalScrollBar()->value() + diff.y() );
}

void ScrollArea::paintBlocks() {
    QPixmap pixmap = QPixmap::fromImage( mImage );
    QPainter painter( &pixmap );
    painter.setRenderHint( QPainter::Antialiasing, true );

    QRect first = QRect( mFirstBlock, mFirstBlock + QPoint( Block::size, Block::size ) );
    painter.setPen( QPen( Qt::black, 3 ) );
    painter.drawRect( first );
    painter.setPen( QPen( Qt::cyan, 2 ) );
    painter.drawRect( first );

    QRect second = QRect( mSecondBlock, mSecondBlock + QPoint( Block::size, Block::size ) );
    painter.setPen( QPen( Qt::black, 3 ) );
    painter.drawRect( second );
    painter.setPen( QPen( Qt::red, 2 ) );
    painter.drawRect( second );

    mLabel->setPixmap( pixmap );
}

void ScrollArea::slotDrawImage( QImage image, bool fit ) {
    Q_ASSERT( image.format() == QImage::Format_ARGB32_Premultiplied );

    mImage = image;
    paintBlocks();

    if( fit ) {
        mLabel->adjustSize();
        autoZoom();
    }

    mLastRect = this->mLabel->rect();
}

void ScrollArea::slotSetFirstBlock( const PointI pos ) {
    mFirstBlock.setX( pos.x() );
    mFirstBlock.setY( pos.y() );
    paintBlocks();
}

void ScrollArea::slotSetSecondBlock( const PointI pos ) {
    mSecondBlock.setX( pos.x() );
    mSecondBlock.setY( pos.y() );
    paintBlocks();
}

void ScrollArea::centerZoom() {
    QPointF diff;

    int dw = mLabel->rect().width() - mLastRect.width();
    diff.setX( dw / 2 );

    int dh = mLabel->rect().height() - mLastRect.height();
    diff.setY( dh / 2 );

    scrollBy( diff );
    mLastRect = mLabel->rect();
}

void ScrollArea::wheelEvent( QWheelEvent* event ) {

    if( mLabel->pixmap() ) {

        this->setWidgetResizable( false );
        float delta = event->delta();

        // clip range from -1..1
        delta /= 120.f;
        delta = std::min( std::max( delta, -1.f ), 1.f );

        // zoom factor 1/25
        delta /= 25.f;

        mZoom += mZoom * delta;
        this->zoom();
    }
}

void ScrollArea::mouseDoubleClickEvent( QMouseEvent* ) {
    this->autoZoom();
}

void ScrollArea::mousePressEvent( QMouseEvent* event ) {
    mMousePressed = true;
    mMousePosition = event->pos();
    update();
}

void ScrollArea::mouseReleaseEvent( QMouseEvent* ) {
    mMousePressed = false;
    update();
}

void ScrollArea::mouseMoveEvent( QMouseEvent* event ) {
    if( mMousePressed ) {

        // get inverted delta
        QPointF position = event->pos();
        QPointF diff = mMousePosition - position;

        // scroll by delta
        scrollBy( diff );

        // update
        mMousePosition = position;
        update();
    }
}

void ScrollArea::contextMenu( const QPoint& pos ) {

    if( mImage.isNull() ) {
        return;
    }

    QPointF imagePos = pos - mLabel->pos();
    imagePos /= mZoom;
    qDebug() << imagePos;

    QMenu menu;
    QAction* first  = menu.addAction( tr( "Set first block" ) );
    QAction* second = menu.addAction( tr( "Set second block" ) );

    QAction* selection = menu.exec( mapToGlobal( pos ) );

    if( selection == first ) {
        mFirstBlock = imagePos.toPoint();
        emit signalSendFirstBlock( imagePos.toPoint() );
    }

    if( selection == second ) {
        mSecondBlock = imagePos.toPoint();
        emit signalSendSecondBlock( imagePos.toPoint() );
    }

    paintBlocks();
    mMousePressed = false;
}

void ScrollArea::zoom() {
    // LOG( "Zoom : " + std::to_string( mZoom ) );
    mLabel->resize( mZoom * mLabel->pixmap()->size() );
    centerZoom();
}
