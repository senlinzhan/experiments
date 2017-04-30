/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */

#ifndef _SHOWIMAGEWINDOW_H_
#define _SHOWIMAGEWINDOW_H_

#include <QMainWindow>

class QLabel;
class QToolButton;
class QToolBar;
class QAction;
class QScrollArea;

class ShowImageWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ShowImageWindow( QPixmap pixmap, QWidget * = 0 );
    void setGrabImage( QPixmap );
    void saveGrabImage();
    bool imageIsSaved();

protected:
    void resizeEvent( QResizeEvent * )    override;
    void closeEvent( QCloseEvent * )      override;
    void mousePressEvent( QMouseEvent * ) override;
    void mouseMoveEvent( QMouseEvent * )  override;

private:
    void createComponents();
    void buildConnections();
    void fitToWindow( bool );
    void rotateImage( qreal );
    void scaleImage( qreal );
    void updateGrabImage();
    void updateScaleFactor();
    void updateZoomButton();
    
    QLabel *imageLabel_;
    QToolButton *save_;
    QToolButton *rotateLeft_;
    QToolButton *rotateRight_;
    QToolButton *zoomIn_;
    QToolButton *zoomOut_;
    QToolButton *fullScreen_;
    QToolButton *about_;
    QToolBar *controlBar_;
    QAction *copyImage_;
    QScrollArea *scroll_;
    
    QPixmap pixmap_;
    QPoint dragStartPos_;
    qreal scaleFactor_ = 1.0;
    qreal rotateDegree_ = 0.0;
    bool imageSaved_ = false;
    
    constexpr static int WINDOW_WIDTH  = 600;
    constexpr static int WINDOW_HEIGHT = 400;
    constexpr static qreal RIGHT_ROTATE_DEGREE = 90;
    constexpr static qreal LEFT_ROTATE_DEGREE = 90;
    constexpr static qreal ZOOM_IN_FACTOR = 1.25;
    constexpr static qreal ZOOM_OUT_FACTOR = 0.75;
    constexpr static qreal MAX_ZOOM = 3.0;
    constexpr static qreal MIN_ZOOM = 0.3;
};

#endif
