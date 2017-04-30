/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */

#include "ShowImageWindow.hpp"
#include <QtWidgets>

ShowImageWindow::ShowImageWindow( QPixmap pixmap, QWidget *parent )
    : QMainWindow( parent ), pixmap_( pixmap )
{
    createComponents();
    buildConnections();
    updateGrabImage();
    fitToWindow( true );

    setAttribute( Qt::WA_DeleteOnClose );    
    resize( WINDOW_WIDTH, WINDOW_HEIGHT );
    setWindowTitle( u8"截图助手" );
}

void ShowImageWindow::createComponents() 
{
    imageLabel_ = new QLabel;
    imageLabel_->setAlignment( Qt::AlignCenter );
    imageLabel_->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    scroll_ = new QScrollArea;
    scroll_->setWidget( imageLabel_ );
    setCentralWidget( scroll_ );

    copyImage_ = new QAction( u8"复制到剪切版", imageLabel_ );
    imageLabel_->addAction( copyImage_ );
    imageLabel_->setContextMenuPolicy( Qt::ActionsContextMenu );
    
    save_ = new QToolButton;
    save_->setIcon( QIcon(":images/save.png") );
    save_->setToolTip( u8"保存图片" );

    rotateLeft_ = new QToolButton;
    rotateLeft_->setIcon( QIcon( ":images/left.png" ) );
    rotateLeft_->setToolTip( u8"左旋转" );
    
    rotateRight_ = new QToolButton;
    rotateRight_->setIcon( QIcon( ":images/right.png" ) );
    rotateRight_->setToolTip( u8"右旋转" );

    zoomIn_ = new QToolButton;
    zoomIn_->setIcon( QIcon( ":images/zoom-in.png" ) );
    zoomIn_->setToolTip( u8"放大图片" );

    zoomOut_ = new QToolButton;
    zoomOut_->setIcon( QIcon( ":images/zoom-out.png" ) );
    zoomOut_->setToolTip( u8"缩小图片" );
    
    fullScreen_ = new QToolButton;
    fullScreen_->setIcon( QIcon( ":images/full.png" ) );
    fullScreen_->setToolTip( u8"使图片适应窗口" );
    
    about_ = new QToolButton;
    about_->setIcon( QIcon( ":images/about.png" ) );
    about_->setToolTip( u8"关于截图助手" );

    controlBar_ = new QToolBar;
    controlBar_->addWidget( save_ );
    controlBar_->addSeparator();
    controlBar_->addWidget( rotateLeft_ );
    controlBar_->addWidget( rotateRight_ );
    controlBar_->addSeparator();
    controlBar_->addWidget( zoomIn_ );
    controlBar_->addWidget( zoomOut_ );
    controlBar_->addSeparator();
    controlBar_->addWidget( fullScreen_ );
    controlBar_->addSeparator();
    controlBar_->addWidget( about_ );

    addToolBar( controlBar_ );
}

void ShowImageWindow::buildConnections()
{
    connect( save_, &QToolButton::clicked, this, 
             &ShowImageWindow::saveGrabImage );
    connect( rotateLeft_, &QToolButton::clicked, this, 
             [=] {  rotateImage( LEFT_ROTATE_DEGREE );  } );
    connect( rotateRight_, &QToolButton::clicked, this, 
             [=] {  rotateImage( RIGHT_ROTATE_DEGREE );  } );
    connect( zoomIn_, &QToolButton::clicked, this, 
             [=] {  scaleImage( ZOOM_IN_FACTOR );  } );
    connect( zoomOut_, &QToolButton::clicked, this,
             [=] {  scaleImage( ZOOM_OUT_FACTOR );  } );
    connect( copyImage_, &QAction::triggered, this, 
             [=] {
                 if( imageLabel_->pixmap() != nullptr ) {
                     QApplication::clipboard()->setPixmap( *imageLabel_->pixmap() );
                 } } );
    connect( fullScreen_, &QToolButton::clicked, this, 
             [=] {  fitToWindow( true );  } );
    connect( about_, &QToolButton::clicked, this, 
             [=] {
                 QMessageBox::about( this, u8"关于截图助手", u8"<img src=':images/logo.png'>"
                                     "<h2>截图助手v0.1</h2>"
                                     "<h3>作者：<font color=blue>詹春畅</font></h3>"
                                     "<h3>主页：<a href='https://senlinzhan.github.io'>senlinzhan.github.io</a></h3>" );
             });
}

void ShowImageWindow::fitToWindow( bool isFit )
{
    scroll_->setWidgetResizable( isFit );
    if( isFit ) {
        QSize originSize = pixmap_.size();
        QSize size = originSize.scaled( imageLabel_->size(), Qt::KeepAspectRatio );
        scaleFactor_ = 1.0 * size.width() / originSize.width();
        updateGrabImage();
    }
}

void ShowImageWindow::updateGrabImage()
{
    static QMatrix matrix;
    fitToWindow( false );
    QPixmap adjustedPixmap = pixmap_.scaled( pixmap_.size() * scaleFactor_, 
                                             Qt::KeepAspectRatio, Qt::SmoothTransformation );
    if( rotateDegree_ != 0.0 ) {
        matrix.rotate( rotateDegree_ );
        adjustedPixmap = adjustedPixmap.transformed( matrix, Qt::SmoothTransformation );
        rotateDegree_ = 0.0;
    }
    imageLabel_->resize( adjustedPixmap.size() );
    imageLabel_->setPixmap( adjustedPixmap );
    updateZoomButton();
}

void ShowImageWindow::rotateImage( qreal degree )
{
    rotateDegree_ = degree;
    updateGrabImage();
}

void ShowImageWindow::scaleImage( qreal scale )
{
    qreal newFactor = scaleFactor_ * scale;
    if( newFactor > MIN_ZOOM && newFactor < MAX_ZOOM ) {
        scaleFactor_ = newFactor;
        updateGrabImage();
    }
}

void ShowImageWindow::updateZoomButton()
{
    if( scaleFactor_ * ZOOM_IN_FACTOR > MAX_ZOOM ) {
        zoomIn_->setEnabled( false );
    } else {
        zoomIn_->setEnabled( true );
    }
    if( scaleFactor_ * ZOOM_OUT_FACTOR < MIN_ZOOM ) {
        zoomOut_->setEnabled( false );
    } else {
        zoomOut_->setEnabled( true );
    }
}

void ShowImageWindow::closeEvent( QCloseEvent *event )
{
    if( !imageSaved_ ) {
         if( !QMessageBox::question( this, u8"丢弃文件？", u8"文件尚未保存，需要保存吗？", 
                                     u8"保存", u8"不保存" ) ) {
             saveGrabImage();
             if( !imageSaved_ ) {
                 event->ignore();
             } else {
                 event->accept();
             }
         } else {
             event->accept();
         }
    }
}

void ShowImageWindow::mousePressEvent( QMouseEvent *event ) 
{
   if( event->button() == Qt::LeftButton
       && imageLabel_->geometry().contains( event->pos() ) 
        && imageLabel_->pixmap() != nullptr ) {
       dragStartPos_ = event->pos();
   }
}

void ShowImageWindow::mouseMoveEvent( QMouseEvent *event ) 
{
    if ( !(event->buttons() & Qt::LeftButton ) ) {
        return;
    }
    if( (event->pos() - dragStartPos_).manhattanLength() < QApplication::startDragDistance() ) {
        return;
    }
    QDrag *drag = new QDrag( this );
    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData( QVariant(*imageLabel_->pixmap()) );
    drag->setMimeData( mimeData );
    drag->setPixmap( imageLabel_->pixmap()->scaled( 300, 300, Qt::KeepAspectRatio, 
                                                    Qt::SmoothTransformation ) );
    drag->exec();
}

void ShowImageWindow::saveGrabImage() 
{
     QString format = "png";
     QString initialPath = QDir::currentPath() + u8"/未命名." + format;      
     QString fileName = QFileDialog::getSaveFileName( this, u8"另存为", 
                                                      initialPath,
                                                      tr( "%1 Files (*.%2);;All Files (*)" )
                                                      .arg( format.toUpper() )
                                                      .arg( format ) );
     
     if( !fileName.isEmpty() ) {
         pixmap_.save( fileName, format.toLatin1().constData() );
         imageSaved_ = true;
     }
}

void ShowImageWindow::setGrabImage( QPixmap pixmap )
{
    pixmap_ = pixmap;
    updateGrabImage();
    imageSaved_ = false;
}

void ShowImageWindow::resizeEvent( QResizeEvent * )
{
    updateScaleFactor();
}

void ShowImageWindow::updateScaleFactor() 
{
    if( imageLabel_->pixmap() != nullptr ) {
        QSize actualSize = imageLabel_->pixmap()->size();
        QSize originSize = pixmap_.size();
        scaleFactor_ = 1.0 * actualSize.width() / originSize.width();
    }
}

bool ShowImageWindow::imageIsSaved() 
{
    return imageSaved_;
}
