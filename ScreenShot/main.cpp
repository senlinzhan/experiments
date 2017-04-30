#include "ScreenShotWindow.hpp"
#include "GrabArea.hpp"
#include <QApplication>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    ScreenShotWindow window;
    window.show();
    
    return app.exec();
}



















