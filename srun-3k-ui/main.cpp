#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    MainWindow w;
    w.setWindowTitle("校园网登陆器");
    w.show();
    return a.exec();
}
