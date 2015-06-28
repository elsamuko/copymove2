#pragma once

#include <QScrollArea>

class QLabel;

class ScrollArea : public QScrollArea {
        Q_OBJECT
    public:
        explicit ScrollArea( QWidget* parent = 0 );
        void setImage( const QImage image );

    signals:

    public slots:

    protected:
        virtual void wheelEvent( QWheelEvent* event );
        virtual void mouseDoubleClickEvent( QMouseEvent* );

    private:
        void zoom();
        void autoZoom();

        float mZoom;
        QLabel* mLabel;
        QImage mImage;
};
