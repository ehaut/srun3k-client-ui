#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//高DPI屏支持
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("校园网登陆器");
    w.show();
    return a.exec();
}
