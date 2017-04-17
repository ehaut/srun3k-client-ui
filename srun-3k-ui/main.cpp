#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("校园网登陆器 By CHN-STUDENT V0.0.2");
    w.show();
    return a.exec();
}
