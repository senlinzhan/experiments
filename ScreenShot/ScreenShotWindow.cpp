/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */

#include "ScreenShotWindow.hpp"
#include "ShowImageWindow.hpp"
#include "GrabArea.hpp"
#include <QtWidgets>

ScreenShotWindow::ScreenShotWindow( QWidget *parent )
    : QWidget( parent )
{
    createWidgets();
    manageLayouts();
    buildConnections();

    setFixedSize( WINDOW_WIDTH, WINDOW_HEIGHT );
    setWindowTitle( u8"截图助手" );
    QDesktopWidget *desktop = QApplication::desktop();
    move( (desktop->width() - width()) / 2, (desktop->height() - height()) / 2 );
}

void ScreenShotWindow::createWidgets()
{
    grabDesktop_ = new QRadioButton( u8"抓取整个桌面" );
    grabCurrentWin_ = new QRadioButton( u8"抓取当前窗口" );
    grabArea_ = new QRadioButton( u8"选择一个截取区域" );
    delayLabel_ = new QLabel( u8"抓取前的延迟(秒)" );
    delaySeconds_ = new QSpinBox;
    grab_ = new QPushButton( u8"截图" );

    delaySeconds_->setRange( 0, 60 );
    grabDesktop_->setChecked( true );
}

void ScreenShotWindow::manageLayouts()
{
    buttonLayout_ = new QGridLayout;    
    buttonLayout_->addWidget( grabDesktop_, 0, 0 );
    buttonLayout_->addWidget( grabCurrentWin_, 1, 0 );
    buttonLayout_->addWidget( grabArea_, 2, 0 );
    buttonLayout_->addWidget( delayLabel_, 3, 0 );
    buttonLayout_->addWidget( delaySeconds_, 3, 1 );

    groupBox_ = new QGroupBox( u8"选项" );
    groupBox_->setLayout( buttonLayout_ );
    
    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget( groupBox_ );
    mainLayout_->addWidget( grab_ );

    setLayout( mainLayout_ );
}

void ScreenShotWindow::buildConnections()
{
    connect( grabArea_, &QRadioButton::toggled, this, [=]{  setDelayEnable( false );  });
    connect( grabDesktop_, &QRadioButton::toggled, this, [=]{  setDelayEnable( true );  });
    connect( grabCurrentWin_, &QRadioButton::toggled, this, [=]{  setDelayEnable( true );  });
    connect( grab_, &QPushButton::clicked, this, &ScreenShotWindow::startGrab );
}

void ScreenShotWindow::startGrab()
{
    int timeOffsetSeconds = 1;
    if( grabDesktop_->isChecked() ) {
        hide();
        QTimer::singleShot( (delaySeconds_->value() + timeOffsetSeconds) * 1000 ,
                            this, SLOT(shotDesktop()) );
    } else if( grabCurrentWin_->isChecked() ) {
        hide();
        QTimer::singleShot( (delaySeconds_->value() + timeOffsetSeconds) * 1000, 
                            this, SLOT(shotCurrentWindow()) );
    } else {
        shotArea();
    }
}

void ScreenShotWindow::shotArea()
{
    GrabArea *grab = new GrabArea();
    grab->show();
    connect( grab, &GrabArea::grabFinished, this, [=] {
            QPixmap grabPixmap = grab->getGrabPixmap();
            showImage( grabPixmap );
            delete grab;
        });
}

void ScreenShotWindow::shotDesktop()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap;
    if( screen ) {
        pixmap = screen->grabWindow( 0 );
    }
    showImage( pixmap );
    show();
}

void ScreenShotWindow::shotCurrentWindow()
{
    QWidget *widget = QApplication::activeWindow();
    if( widget != nullptr ) {
        QPixmap p = QPixmap::grabWidget( widget );
        p.save("a.png");
    } else {
        qDebug() << "null";
        
    }
    show();
} 

void ScreenShotWindow::showImage( QPixmap pixmap )
{
    ShowImageWindow *imageWindow = new ShowImageWindow( pixmap );
    imageWindow->show();
    QDesktopWidget *desktop = QApplication::desktop();
    imageWindow->move( (desktop->width() - imageWindow->width()) / 2, 
                       (desktop->height() - imageWindow->height()) / 2 );   
}

void ScreenShotWindow::setDelayEnable( bool enable )
{
     delayLabel_->setDisabled( !enable );
     delaySeconds_->setEnabled( enable );
}  

