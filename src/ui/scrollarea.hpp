#pragma once

#include <QScrollArea>

#include "point.hpp"

class QLabel;

class ScrollArea : public QScrollArea {
        Q_OBJECT
    public:
        explicit ScrollArea( QWidget* parent = 0 );

    signals:
        void signalSendFirstBlock( QPoint pos );
        void signalSendSecondBlock( QPoint pos );

    public slots:
        void slotDrawImage( QImage image , bool fit );
        void slotSetFirstBlock( const PointI pos );
        void slotSetSecondBlock( const PointI pos );

    protected:
        virtual void wheelEvent( QWheelEvent* event );
        virtual void mouseDoubleClickEvent( QMouseEvent* );
        virtual void mousePressEvent( QMouseEvent* event );
        virtual void mouseReleaseEvent( QMouseEvent* );
        virtual void mouseMoveEvent( QMouseEvent* );

    private slots:
        void contextMenu( const QPoint& pos );

    private:
        void zoom();
        void autoZoom();
        void centerZoom();
        void scrollBy( const QPointF& diff );
        void paintBlocks();
        void checkBlockGrab();
        void releaseBlockGrab();
        QPointF toRelativePosition( const QPointF& absolute );

        float mZoom;
        QLabel* mLabel;
        QImage mImage;

        QPoint mFirstBlockTopLeft;
        QPoint mSecondBlockTopLeft;
        bool mFirstBlockGrabbed;
        bool mSecondBlockGrabbed;

        bool mMousePressed;
        QPointF mMousePosition;
        QRect mLastRect;
};
