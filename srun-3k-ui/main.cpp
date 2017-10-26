#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //汉化右键菜单，参考http://blog.csdn.net/abcvincent/article/details/71247111?locationNum=16&fps=1
    QTranslator translator;
       qApp->installTranslator(&translator);
       translator.load(":/language/qt_zh_CN.qm");
       //添加qt_zh_CN.qm汉化；

       QTranslator translator2;
       qApp->installTranslator(&translator2);
       translator2.load(":/language/widgets.qm");
       //添加widgets.qm汉化；
    MainWindow w;
    w.show();
    return a.exec();
}
