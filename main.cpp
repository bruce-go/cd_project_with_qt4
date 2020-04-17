#include <QApplication>//头文件中声明了class QDesktopWidget;但是并没有包含这个类相关的头文件，
// 所以必须在下面包含这个头文件，app->desktop()才能正确调用QDesktopWidget类的成员
#include <iostream>

#include <QDesktopWidget>//如果不包含这个头文件，会报错error: invalid use of incomplete type ‘class QDesktopWidget’

#include "mainwindow.h"
#include "cd_dbm.h"

MainWindow *mainWin;
QApplication *app;

int main(int argc, char *argv[])
{
    int rtnVal;
    //QApplication app(argc, argv);
    app = new QApplication(argc, argv);

    rtnVal = database_initialize(0);
    if(!rtnVal)
    {
        std::cerr << "Error, unable to initialize database!" << std::endl;
    }
    
    mainWin = new MainWindow();

    const QRect screen = app->desktop()->screenGeometry(mainWin);
    mainWin->move(screen.center() - mainWin->rect().center());
    /*给mainWindow设置一个合适的初始大小，并让其位于桌面的中央*/
    
    mainWin->show();
    return app->exec();
}
