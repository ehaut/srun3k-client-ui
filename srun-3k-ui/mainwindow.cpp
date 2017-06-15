#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMouseEvent"
#include "qt_windows.h"
#include "QDesktopServices"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏
    QPushButton *closeButton= new QPushButton(this);//建立关闭按钮
    connect(closeButton, SIGNAL(clicked()), this, SLOT(Close()));//连接关闭信号
    closeButton->setGeometry(305,0,43,33);
    closeButton->setToolTip(tr("关闭"));//设置鼠标移至按钮上的提示信息
   closeButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/close);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/close_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/close_pressed);}");//设置最小化、关闭等按钮的样式
   QPushButton *AboutButton= new QPushButton(this);//建立关闭按钮
   connect(AboutButton, SIGNAL(clicked()), this, SLOT(on_ABOUT_clicked()));//连接信号
   AboutButton->setToolTip(tr("关于"));
   AboutButton->setGeometry(340,490,10,10);
   AboutButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/about);}");
   QPushButton *ServiceButton= new QPushButton(this);//建立关闭按钮
   connect(ServiceButton, SIGNAL(clicked()), this, SLOT(on_SERVICE_clicked()));//连接信号
   ServiceButton->setToolTip(tr("自服务"));
   ServiceButton->setGeometry(125,80,100,100);
   ServiceButton->setStyleSheet("QPushButton {border-image: url(:/Ico/R);}");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Close()
{
    close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{//窗口拖动，参考http://blog.csdn.net/aqtata/article/details/8902889
    if (ReleaseCapture())
        SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setText("登陆中...");
    ui->pushButton->setText("密码错误！");
    ui->pushButton->setStyleSheet(" background: #ff0000;"
                                  "border: 0;"
                                  "border-radius: 3px;"
                                  "line-height: 41px;"
                                  "color: #fff;");
    ui->pushButton->setText("登陆成功！");
    ui->pushButton->setStyleSheet(" background: #00aa00;"
                                  "border: 0;"
                                  "border-radius: 3px;"
                                  "line-height: 41px;"
                                  "color: #fff;");
}

void MainWindow::on_ABOUT_clicked()
{//关于按钮
    QDesktopServices::openUrl(QUrl("http://https://github.com/CHN-STUDENT/SRUN-3K-UI"));
}

void MainWindow::on_SERVICE_clicked()
{//自服务按钮
    QDesktopServices::openUrl(QUrl("http://172.16.154.130:8800/"));
}
