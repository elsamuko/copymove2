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

    private:
        void zoom();
        float mZoom;
        QLabel* mLabel;
};
