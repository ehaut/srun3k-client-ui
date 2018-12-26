#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

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
    connect(ui->messageButtonInLogoutPage, SIGNAL(clicked()),this, SLOT(showMessage()));
    connect(ui->messageButtonInLoginPage, SIGNAL(clicked()),this, SLOT(showMessage()));
    connect(ui->backButtonInadvanceSettingsPage, SIGNAL(clicked()),this, SLOT(on_ENTER_clicked()));
    connect(ui->enterInMessagePage, SIGNAL(clicked()),this, SLOT(on_ENTER_clicked()));
    connect(ui->enterButtonInAboutPage, SIGNAL(clicked()),this, SLOT(on_ENTER_clicked()));
    connect(ui->serviceButtonInLoginPage, SIGNAL(clicked()),this, SLOT(on_SERVICE_clicked()));
    connect(ui->serviceButtonInLogoutPage, SIGNAL(clicked()),this, SLOT(on_SERVICE_clicked()));
    ui->usernameLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->passwordLineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->stackedWidget->setCurrentIndex(2);
    AboutButton->hide();//在获取公告前隐藏这两个按钮
    AdvancedButton->hide();
    n=new network();
    s=new storage();
    ui->Ladvanced->setText("初始化设置：");
    ui->backButtonInadvanceSettingsPage->hide();
    isServerinfoExist=s->readServerConfigFromFile(serverConfig);
    if(isServerinfoExist)
    {

        ui->Ladvanced->setText("高级设置：");
        ui->loginServerLineEdit->setText(serverConfig.at(0));
        ui->serviceServerLineEdit->setText(serverConfig.at(1));
        ui->macLineEdit->setText(serverConfig.at(2));
        ui->loginServerPortSpinbox->setValue(serverConfig.at(3).toInt());
        ui->serviceServerPortSpinbox->setValue(serverConfig.at(4).toInt());
        ui->acidSpinBox->setValue(serverConfig.at(5).toInt());
        ui->typeSpinBox->setValue(serverConfig.at(6).toInt());
        ui->dropSpinBox->setValue(serverConfig.at(7).toInt());
        ui->popSpinBox->setValue(serverConfig.at(8).toInt());
        ui->backButtonInadvanceSettingsPage->show();
        getServerInfo();
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(4);
        on_setDefaultButton_clicked();
    }
    isUserinfoExists=s->readuserConfigFromFile(userConfig);

}

void MainWindow::showMessage(void)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_ABOUT_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_ADVANCED_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_ENTER_clicked()
{
    if(isOnline)
        ui->stackedWidget->setCurrentIndex(3);
    else
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void MainWindow::getServerInfo(void)
{
     ui->statusBar->setText("获取服务器信息中...");
     setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
     ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                     "QWidget#widgetBottom{background:#3498DB;}");
     QString url=serverConfig.at(0)+"/v2/srun_portal_message";
     QStringList serverMessage=n->parseServerMessage(n->httpGet(url.toStdString().c_str()));
     AboutButton->show();
     AdvancedButton->show();
     connect(AboutButton, SIGNAL(clicked()), this, SLOT(on_ABOUT_clicked()));//连接信号
     connect(AdvancedButton, SIGNAL(clicked()), this, SLOT(on_ADVANCED_clicked()));//连接信号
     if(serverMessage.size()!=0||!serverMessage.isEmpty())
     {
         ui->statusBar->setText("获取服务器信息中...成功！");
         setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
         ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                         "QWidget#widgetBottom{background:#1aad18;}");
        ui->messageLable->setText(serverMessage.at(0));
        ui->messageBox->setText(serverMessage.at(1));

     }
     else
     {
         ui->statusBar->setText("获取服务器信息中...服务器连接失败！");
         setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
         ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                         "QWidget#widgetBottom{background:#E05D6F;}");
     }
     getUserInfo();
}


void MainWindow::getUserInfo(void)
{
    ui->statusBar->setText("获取用户状态中...");
    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
    ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                    "QWidget#widgetBottom{background:#3498DB;}");
    QString url=serverConfig.at(0)+":"+serverConfig.at(3)+"/cgi-bin/rad_user_info";

    QStringList list=n->parseUserInfo(n->httpGet(url.toStdString().c_str()),usedtime,isOnline);
    ui->statusBar->setText("获取用户状态中...成功！");
    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
    ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                    "QWidget#widgetBottom{background:#1aad18;}");

    if(!isOnline)
    {
        ui->stackedWidget->setCurrentIndex(2);
        if(ui->autoLoginCheckBox->isChecked())
         {
             QTimer::singleShot(30,[this](){ui->loginButton->click();});
         }
    }
    else
    {
        /**
            list[0] username
            list[1] ip
            list[2] data
            list[3] time
        */
        QString name=ui->nameLable->text();
        name=name.arg(list.at(0));
        logoutname=list.at(0);
        ui->nameLable->setText(name);
        QString ip=ui->ipLable->text();
        ip=ip.arg(list.at(1));
        ui->ipLable->setText(ip);
        QString data=ui->dataLable->text();
        data=data.arg(list.at(2));
        ui->dataLable->setText(data);
        QString time="时    间：%1 小时 %2 分 %3 秒";
        time=time.arg(QString::number(usedtime/3600,10)).arg(QString::number(usedtime/60%60,10)).arg(QString::number(usedtime%60,10));
        ui->timeLable->setText(time);
        if(meTimer==nullptr)
        {
            meTimer=new QTimer(this);  //this说明是当前类对象的定时器
            connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
        }
        meTimer->start(1000);
        ui->stackedWidget->setCurrentIndex(3);
    }
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
    connect(mAboutAction,SIGNAL(triggered()),this,SLOT(on_mABOUT_clicked()));
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

void MainWindow::on_SERVICE_clicked()
{
    ui->statusBar->setText("打开自服务中...");
    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
    ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                    "QWidget#widgetBottom{background:#3498DB;}");
    QString url=serverConfig.at(1)+":"+serverConfig.at(4);
    //qDebug()<<url;
    QDesktopServices::openUrl(QUrl(url));
    ui->statusBar->setText("打开自服务中...成功！");
    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
    ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                    "QWidget#widgetBottom{background:#1aad18;}");
}

void MainWindow::on_mABOUT_clicked()
{
    on_showMainAction();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_showMainAction()
{//按下显示主界面
    mSysTrayIcon->deleteLater();
    mMenu->deleteLater();
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
    if(meTimer!=nullptr)
    {
        meTimer->stop();
        delete meTimer;
    }
    delete n;
    delete s;
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
    ui->enterButtonInAboutPage->setEnabled(false);
    ui->checkUpdateButton->setEnabled(false);
    if(isOnline)
    {
        QStringList list;
        ui->statusBar->setText("检查更新中...");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#3498DB;}");
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
            ui->stackedWidget->setCurrentIndex(0);
            ui->statusBar->setText("检查更新中...服务器连接失败！");
            setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
            ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                            "QWidget#widgetBottom{background:#E05D6F;}");
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->statusBar->setText("检查更新中...服务器连接失败！");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#E05D6F;}");
    }
    ui->enterButtonInAboutPage->setEnabled(true);
    ui->checkUpdateButton->setEnabled(true);
}

void MainWindow::on_setDefaultButton_clicked()
{
//    static QString serverSettings="loginServerLineEdit"
//            "serviceServerLineEdit"
//            "macLineEdit"
//            "loginServerPortSpinbox"
//            "serviceServerPortSpinbox"
//            "acidSpinBox"
//            "typeSpinBox"
//            "dropSpinBox"
//            "popSpinBox";
    ui->loginServerLineEdit->setText("http://172.16.154.130");
    ui->serviceServerLineEdit->setText("http://172.16.154.130");
    ui->macLineEdit->setText("02:00:00:00:00:00");
    ui->loginServerPortSpinbox->setValue(69);
    ui->serviceServerPortSpinbox->setValue(8800);
    ui->acidSpinBox->setValue(1);
    ui->typeSpinBox->setValue(3);
    ui->dropSpinBox->setValue(0);
    ui->popSpinBox->setValue(1);
}

void MainWindow::on_saveButtonInadvanceSettingsPage_clicked()
{
     ui->statusBar->setText("保存服务器信息中...");
     setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
     ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                     "QWidget#widgetBottom{background:#3498DB;}");
     serverConfig.clear();
     serverConfig<<(ui->loginServerLineEdit->text().toUtf8());
     serverConfig<<(ui->serviceServerLineEdit->text().toUtf8());
     serverConfig<<(ui->macLineEdit->text().toUtf8());
     serverConfig<<(ui->loginServerPortSpinbox->text().trimmed());
     serverConfig<<(ui->serviceServerPortSpinbox->text().trimmed());
     serverConfig<<(ui->acidSpinBox->text().trimmed());
     serverConfig<<(ui->typeSpinBox->text().trimmed());
     serverConfig<<(ui->dropSpinBox->text().trimmed());
     serverConfig<<(ui->popSpinBox->text().trimmed());
     if(s->saveserverConfigToFile(serverConfig))
     {
         ui->statusBar->setText("保存服务器信息中...成功！");
         setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
         ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                         "QWidget#widgetBottom{background:#1aad18;}");
         getServerInfo();
          ui->Ladvanced->setText("高级设置：");
         ui->backButtonInadvanceSettingsPage->show();
     }
     else
     {
         ui->statusBar->setText("保存服务器信息中...失败！");
         setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
         ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                         "QWidget#widgetBottom{background:#E05D6F;}");
     }
}


void MainWindow::TimeSlot()
{
    QString time="时    间：%1 小时 %2 分 %3 秒";
    time=time.arg(QString::number(usedtime/3600,10)).arg(QString::number(usedtime/60%60,10)).arg(QString::number(usedtime%60,10));
    ui->timeLable->setText(time);
    usedtime++;
}

void MainWindow::on_RetryButton_clicked()
{
    QTimer::singleShot(2000, this, SLOT(getServerInfo()));
}

void MainWindow::on_logoutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
//    ui->logoutButton->setEnabled(false);
//    ui->statusBar->setText("注销中...");
//    setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #3498DB;}");
//    ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
//                                    "QWidget#widgetBottom{background:#3498DB;}");
//    QString post="&action=logout&ac_id=";
//    QString nameEncrypt;
//    nameEncrypt.append(s->usernameEncrypt(logoutname));
//    post=post+serverConfig.at(5)+"&mac="+serverConfig.at(2)+"&type="+serverConfig.at(6)+"&username=%7BSRUN3%7D%0D%0A"+n->urlEncode(nameEncrypt);
//    QString url=serverConfig.at(0)+":"+serverConfig.at(3)+"/cgi-bin/srun_portal";
//    QString reback=n->httpPost(url.toStdString().c_str(),post.toStdString().c_str());
//    qDebug()<<reback;
//    int status;
//    reback=n->parseServerReback(reback,status);
//    if(status==-2)
//    {
//        ui->statusBar->setText("注销中...网络错误！");
//        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
//        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
//                                        "QWidget#widgetBottom{background:#E05D6F;}");
//        ui->stackedWidget->setCurrentIndex(0);
//        isOnline=false;
//    }
//    else if(status==0)
//    {
//        isOnline=false;
//        ui->statusBar->setText("注销中...成功！");
//        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #1aad18;}");
//        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
//                                        "QWidget#widgetBottom{background:#1aad18;}");
//        ui->stackedWidget->setCurrentIndex(2);
//    }
//    else if(reback=="您不在线")
//    {
//        isOnline=false;
//        ui->stackedWidget->setCurrentIndex(2);
//    }
//    ui->logoutButton->setEnabled(true);
}

void MainWindow::on_autoStartCheckBox_clicked()
{
    set=true;
    QSettings *reg=new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(ui->autoStartCheckBox->isChecked())
    {
      reg->setValue("srun3k",QApplication::applicationFilePath().replace("/", "\\"));
    }
    else
    {

      reg->setValue("srun3k","");
    }
}

void MainWindow::on_autoLoginCheckBox_clicked()
{
     set=true;
}

void MainWindow::on_loginButton_clicked()
{
    QString username=ui->usernameLineEdit->text().trimmed();
    QString password=ui->passwordLineEdit->text().trimmed();
    if(username.isEmpty())
    {
        ui->statusBar->setText("请输入您的用户名以便登陆");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#E05D6F;}");
        ui->usernameLineEdit->setFocus();
        return;
    }
    if(password.isEmpty())
    {
        ui->statusBar->setText("请输入您的密码以便登陆");
        setStyleSheet("QWidget#centralWidget{color:black;background:white;border:1px solid #E05D6F;}");
        ui->widgetBottom->setStyleSheet("QLabel#statusBar{color:white;padding:5px 0px 5px;}"
                                        "QWidget#widgetBottom{background:#E05D6F;}");
        ui->passwordLineEdit->setFocus();
        return;
    }

}
