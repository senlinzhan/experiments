/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */

#ifndef _GRABAREA_H_
#define _GRABAREA_H_

#include <QWidget>

class GrabArea : public QWidget
{
    Q_OBJECT
public:
    GrabArea( QWidget * = 0 );
    QPixmap getGrabPixmap() const;

signals:
    void grabFinished();
    
protected:
    void mousePressEvent( QMouseEvent * )   override;
	void mouseMoveEvent( QMouseEvent * )    override;
	void mouseReleaseEvent( QMouseEvent * ) override;
	void paintEvent( QPaintEvent * )        override;
    
private:
    void saveGrabImage();
    
	QPoint startPos_, endPos_;
    QPixmap pixmap_;
    QPixmap grabPixmap_;
    bool isDragging = false;
};

#endif
