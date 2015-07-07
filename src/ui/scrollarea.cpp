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

    this->releaseBlockGrab();

    CHECK_QT_CONNECT( connect( this, &ScrollArea::customContextMenuRequested, this, &ScrollArea::contextMenu, Qt::UniqueConnection ) );
}

void ScrollArea::autoZoom() {

    if( mPixmap.size().isEmpty() ) {
        return;
    }

    float scaledW   = this->size().width();
    float originalW = mPixmap.size().width();

    float scaledH   = this->size().height();
    float originalH = mPixmap.size().height();

    mZoom = std::min( scaledW / originalW, scaledH / originalH ) * 0.99;
    this->zoom();
}

void ScrollArea::scrollBy( const QPointF& diff ) {
    horizontalScrollBar()->setValue( horizontalScrollBar()->value() + diff.x() );
    verticalScrollBar()->setValue( verticalScrollBar()->value() + diff.y() );
}

void ScrollArea::paintBlocks() {

    if( mPixmap.isNull() ) {
        return;
    }

    QPixmap tmp = mPixmap;
    QPainter painter( &tmp );
    painter.setRenderHint( QPainter::Antialiasing, true );

    QRect second = QRect( mSecondBlockTopLeft, mSecondBlockTopLeft + QPoint( Block::size, Block::size ) );
    painter.setPen( QPen( Qt::black, 3 ) );
    painter.drawRect( second );
    painter.setPen( QPen( Qt::red, 2 ) );
    painter.drawRect( second );

    QRect first = QRect( mFirstBlockTopLeft, mFirstBlockTopLeft + QPoint( Block::size, Block::size ) );
    painter.setPen( QPen( Qt::black, 3 ) );
    painter.drawRect( first );
    painter.setPen( QPen( Qt::cyan, 2 ) );
    painter.drawRect( first );

    mLabel->setPixmap( tmp );
}

void ScrollArea::slotDrawImage( QImage image, bool fit ) {
    Q_ASSERT( image.format() == QImage::Format_ARGB32_Premultiplied );

    mPixmap = QPixmap::fromImage( image );
    paintBlocks();

    if( fit ) {
        mLabel->adjustSize();
        autoZoom();
    }

    mLastRect = this->mLabel->rect();
}

void ScrollArea::slotSetFirstBlock( const PointI pos ) {
    mFirstBlockTopLeft.setX( pos.x() );
    mFirstBlockTopLeft.setY( pos.y() );
    paintBlocks();
}

void ScrollArea::slotSetSecondBlock( const PointI pos ) {
    mSecondBlockTopLeft.setX( pos.x() );
    mSecondBlockTopLeft.setY( pos.y() );
    paintBlocks();
}

void ScrollArea::slotZoomActualPixels() {
    mZoom = 1.f;
    this->zoom();
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

void ScrollArea::checkBlockGrab() {
    Q_ASSERT( mMousePressed );

    QPointF relative = this->toRelativePosition( mMousePosition );
    QRectF firstBlock( mFirstBlockTopLeft, QSize( Block::size, Block::size ) );
    QRectF secondBlock( mSecondBlockTopLeft, QSize( Block::size, Block::size ) );

    if( firstBlock.contains( relative ) ) {
        mFirstBlockGrabbed = true;
        mLabel->setCursor( Qt::BlankCursor );
    } else if( secondBlock.contains( relative ) ) {
        mSecondBlockGrabbed = true;
        mLabel->setCursor( Qt::BlankCursor );
    }
}

void ScrollArea::mousePressEvent( QMouseEvent* event ) {
    mMousePressed = true;
    mMousePosition = event->pos();
    checkBlockGrab();
    update();
}

void ScrollArea::releaseBlockGrab() {
    mFirstBlockGrabbed = false;
    mSecondBlockGrabbed = false;
    mLabel->setCursor( Qt::CrossCursor );
}

void ScrollArea::mouseReleaseEvent( QMouseEvent* ) {
    mMousePressed = false;
    releaseBlockGrab();
    update();
}

QPointF ScrollArea::toRelativePosition( const QPointF& absolute ) {
    QPointF relative = absolute - mLabel->pos();
    relative /= mZoom;
    return relative;
}

void ScrollArea::mouseMoveEvent( QMouseEvent* event ) {

    QPointF position = event->pos();

    if( mFirstBlockGrabbed ) {
        mFirstBlockTopLeft = toRelativePosition( position ).toPoint() - QPoint( Block::size / 2, Block::size / 2 );
        emit signalSendFirstBlock( mFirstBlockTopLeft );
        paintBlocks();
    } else if( mSecondBlockGrabbed ) {
        mSecondBlockTopLeft = toRelativePosition( position ).toPoint() - QPoint( Block::size / 2, Block::size / 2 );
        emit signalSendSecondBlock( mSecondBlockTopLeft );
        paintBlocks();
    } else if( mMousePressed ) {
        // get inverted delta
        QPointF diff = mMousePosition - position;
        // scroll by delta
        scrollBy( diff );
    }

    // update
    mMousePosition = position;
    update();
}

void ScrollArea::contextMenu( const QPoint& pos ) {

    if( mPixmap.isNull() ) {
        return;
    }

    QPointF imagePos = this->toRelativePosition( pos );
    qDebug() << imagePos;

    QMenu menu;
    QAction* first  = menu.addAction( tr( "Set first block" ) );
    QAction* second = menu.addAction( tr( "Set second block" ) );

    QAction* selection = menu.exec( mapToGlobal( pos ) );

    if( selection == first ) {
        mFirstBlockTopLeft = imagePos.toPoint();
        emit signalSendFirstBlock( mFirstBlockTopLeft );
    }

    if( selection == second ) {
        mSecondBlockTopLeft = imagePos.toPoint();
        emit signalSendSecondBlock( mSecondBlockTopLeft );
    }

    paintBlocks();
    releaseBlockGrab();
    mMousePressed = false;
}

void ScrollArea::zoom() {
    // LOG( "Zoom : " + std::to_string( mZoom ) );
    mLabel->resize( mZoom * mLabel->pixmap()->size() );
    centerZoom();
}
