/*
 *   Copyright (C) 2014 詹春畅
 *   https://senlinzhan.github.io
 *   截图助手，版本0.1
 *   本程序是自由软件，可以不受限制地自由使用、复制、研究、修改和发布。
 *
 */


#ifndef _SCREENSHOTWINDOW_H_
#define _SCREENSHOTWINDOW_H_

#include <QWidget>
#include <QPixmap>

class QRadioButton;
class QLabel;
class QSpinBox;
class QPushButton;
class QVBoxLayout;
class QGroupBox;
class QGridLayout;

class ScreenShotWindow : public QWidget
{
    Q_OBJECT
public:
    ScreenShotWindow( QWidget* = 0);

private slots:
    void startGrab();
    void shotDesktop();
    void shotCurrentWindow();
    void shotArea();

private:
    void setDelayEnable( bool );
    void createWidgets();
    void buildConnections();
    void manageLayouts();
    void showImage( QPixmap );
    
    QRadioButton *grabDesktop_;
    QRadioButton *grabCurrentWin_;
    QRadioButton *grabArea_;
    QLabel *delayLabel_;
    QSpinBox *delaySeconds_;
    QPushButton *grab_;

    QGridLayout *buttonLayout_;
    QVBoxLayout *mainLayout_;
    QGroupBox *groupBox_;
    
    constexpr static qreal WINDOW_WIDTH = 300;
    constexpr static qreal WINDOW_HEIGHT = 250;
};

#endif
