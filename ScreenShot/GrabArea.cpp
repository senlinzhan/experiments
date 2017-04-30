/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */

#include "GrabArea.hpp"
#include <QtWidgets>

GrabArea::GrabArea( QWidget *parent )
    : QWidget( parent )
{
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint ); 
    setWindowState( Qt::WindowFullScreen ); 
    setCursor( Qt::CrossCursor );
    pixmap_ = QGuiApplication::primaryScreen()->grabWindow( 0 );
}

void GrabArea::mousePressEvent( QMouseEvent *event )
{
    if( event->button() != Qt::LeftButton ) {
        close();
    } else {
        startPos_ = event->pos();
    }
}

void GrabArea::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton ) {
        isDragging = true;
        endPos_ = event->pos();
    } else {
        close();
    }
    update();
}

void GrabArea::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap_ );
    if( isDragging ) {
        painter.setPen( QPen( Qt::darkGray, Qt::DashLine ) );
        painter.setBrush( QColor( 96, 96, 96, 100 ) );
        painter.drawRect( QRect( startPos_, endPos_ ) );
    }
}

void GrabArea::mouseReleaseEvent( QMouseEvent * )
{
    if( isDragging ) {
        isDragging = false;
        if( !QMessageBox::question( this, u8"完成", u8"截图完成！", 
                                    u8"保存截图", u8"重新截取" ) ) {
            saveGrabImage();
            close();
            emit grabFinished();
        }
    }
}

void GrabArea::saveGrabImage() 
{
    grabPixmap_ = QGuiApplication::primaryScreen()->grabWindow(
        0, qMin( startPos_.x(), endPos_.x() ),
        qMin( startPos_.y(), endPos_.y() ),
        qAbs( endPos_.x() - startPos_.x() ),
        qAbs( endPos_.y() - startPos_.y() ) );
}

QPixmap GrabArea::getGrabPixmap() const 
{
    return grabPixmap_;
}
