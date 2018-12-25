#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    mLocation = this->geometry();
    mDrag = false;
    ui->widgetTitle->installEventFilter(this);
    ui->labelTitle->setAlignment(Qt::AlignCenter);
    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #2980b9;}");
    closeButton= new QPushButton(this);//建立关闭按钮
    connect(closeButton, SIGNAL(clicked()), this, SLOT(Close()));//连接关闭信号
    closeButton->setGeometry(595,0,43,33);
    closeButton->setToolTip(tr("关闭"));//设置鼠标移至按钮上的提示信息
    closeButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/close);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/close_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/close_pressed);}");//设置关闭等按钮的样式
    minButton= new QPushButton(this);//建立最小化按钮
    connect(minButton, SIGNAL(clicked()), this, SLOT(Min()));//连接关闭信号
    minButton->setGeometry(550,0,43,33);
    minButton->setToolTip(tr("最小化"));//设置鼠标移至按钮上的提示信息
    minButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/min);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/min_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/min_pressed);}");//设置最小化等按钮的样式
    AboutButton= new QPushButton(this);//建立关于按钮
    AboutButton->setToolTip(tr("关于"));
    AboutButton->setGeometry(610,295,20,20);
    AboutButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/about);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/about_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/about_pressed);}");
    AdvancedButton= new QPushButton(this);//建立高级设置按钮
    AdvancedButton->setToolTip(tr("高级设置"));
    AdvancedButton->setGeometry(580,295,20,20);
    AdvancedButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/advanced);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/advanced_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/advanced_pressed);}");
    connect(ui->aboutBox, SIGNAL(anchorClicked(const QUrl&)),this, SLOT(anchorClickedSlot(const QUrl&)));

    n=new network();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::transparent));
    QColor color(0,0,0,50);
    int arr[10] = {150,120,80,50,40,30,20,10,5,5};
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        if(i == 5)
            path.addRect(10-i-1, 10-i-1, this->width()-(10-i)*2, this->height()-(10-i)*2);
        else
            path.addRoundedRect(10-i-1, 10-i-1, this->width()-(10-i)*2, this->height()-(10-i)*2,2,2);

        color.setAlpha(arr[i]);
        painter.setPen(color);
        painter.drawPath(path);
     }
}


void MainWindow::Min()
{//最小化到托盘，参考http://blog.csdn.net/zhuyunfei/article/details/51433822
    this->hide();
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/Ico/SysIcon");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip("校园网登录器");
    //在系统托盘显示此对象
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
       //连接按下图标之后
    mShowMainAction = new QAction("显示主界面");
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mExitAppAction = new QAction("退出");
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(Close()));
    mServiceAction = new QAction("自服务");
    connect(mServiceAction,SIGNAL(triggered()),this,SLOT(on_SERVICE_clicked()));
    mAboutAction = new QAction("关于");
    connect(mAboutAction,SIGNAL(triggered()),this,SLOT(on_aboutAppAction()));
    mMenu = new QMenu(this);
    //菜单美化参考http://blog.csdn.net/zhouxiao2009/article/details/22984407
    mMenu->setStyleSheet("QMenu {background-color:white;}"
                         "QMenu::item {padding: 2px 25px 2px 25px;border: 1px solid transparent ;}"
                         "QMenu::item:selected:enabled {border-color:#91c9f7;background: #91c9f7;}"
                         "QMenu::separator {height: 2px;background: rgb (168, 216, 235);margin-left: 25px;} "
                         "QMenu::indicator {width: 13px;height: 13px;} ");
    mMenu->addAction(mShowMainAction);
    mMenu->addSeparator();
    mMenu->addAction(mServiceAction);
    mMenu->addSeparator();
    mMenu->addAction(mAboutAction);
    mMenu->addAction(mExitAppAction);
    mSysTrayIcon->setContextMenu(mMenu);
    mSysTrayIcon->show();
}

void MainWindow::on_showMainAction()
{//按下显示主界面
    if(mMenu!=nullptr)
    {
         delete mMenu;
         delete mShowMainAction;
         delete mExitAppAction;
         delete mServiceAction;
         delete mAboutAction;
     }
    mSysTrayIcon->deleteLater();
    mMenu=nullptr;
    this->show();
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{//按下托盘图标
    switch(reason){
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        //双击后显示主程序窗口
        on_showMainAction();
        break;
    default:
        break;
    }
}

void MainWindow::Close()
{
    qApp->exit(0);
}


MainWindow::~MainWindow()
{
   if(mMenu!=nullptr)
   {
        delete mMenu;
        delete mShowMainAction;
        delete mExitAppAction;
        delete mServiceAction;
        delete mAboutAction;
    }
    delete n;
    delete AdvancedButton;
    delete AboutButton;
    delete closeButton;
    delete minButton;
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)//鼠标按下事件
{
    if (e->button() == Qt::LeftButton)
    {
        mDrag = true;
        mDragPos = e->globalPos() - pos();
        e->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)//鼠标移动事件
{
    if (mDrag && (e->buttons() && Qt::LeftButton))
    {
        move(e->globalPos() - mDragPos);
        e->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)//鼠标释放事件
{
    mDrag = false;
}



void MainWindow::anchorClickedSlot(const QUrl &url)
{
     QDesktopServices::openUrl(url);
}


void MainWindow::on_checkUpdateButton_clicked()
{
    QStringList list;
    ui->statusBar->setText("检查更新中...");
    if(checkVersion(list,n)!=-1)
    {
        ui->statusBar->setText("检查更新中...成功！");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#1aad18;}");

        if(checkVersion(list,n)==0)
        {
            QString s="当前版本";
            QString v;
            v.sprintf("%s",version);
            s=s+" "+v+" 已经为最新版本！感谢您的使用！";
            ui->aboutBox->setText(s);
        }
        if(checkVersion(list,n)==1)
        {
            /**
              list[0] version
              list[1] date
              list[2] author
              list[3] sha1
              list[4] url
            */
            QString s="当前版本";
            QString v;
            v.sprintf("%s",version);
            QString ehaut=ui->eHautIco->text();
            QString url=QString("<a href = \"%1\">%2</a>").arg(list.at(4)).arg(ehaut);
            s=s+" "+v+" 。最新版本 "+list.at(0)+" ,发布于 "+list.at(1)+" 。\n\n <=====================\n      点击左边蜗牛下载最新版！ \n\n备用地址："+list.at(4);

            ui->eHautIco->setText(url);
            ui->eHautIco->setOpenExternalLinks(true);
            ui->aboutBox->setText(s);
        }
    }
    else
    {
        ui->statusBar->setText("检查更新中...服务器连接失败！");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#E05D6F;}");
    }
}
