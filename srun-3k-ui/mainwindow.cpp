#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMouseEvent"
#include "qt_windows.h"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QTextCodec"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"
#include "QTimer"
#include "QDesktopServices"
#include "QUrl"
#include "QSettings"
unsigned int usedtime;
QString yourname;
QString login_server;
QString login_port;
QString service_server;
QString service_port;
QString mac;
QString acid;
QString type;
QString drop;
QString pop;
int file_state=0;
int set=0;
int state=0;
/*下面这些是服务器回传信息基准，用于做反馈处理*/
QString login_ok="login_ok,1573330604,0,0,0.0,0.0,0,0,0,0,0,1.01.20160715"; //登陆成功
QString login_ok_short="login_ok";
QString logout_ok="logout_ok";										      //注销成功
QString error1="missing_required_parameters_error";                         //缺少参数
QString error2="login_error#E2553: Password is error.";                     //密码错误
QString error3="login_error#E2531: User not found.";                        //用户未找到
QString error4="login_error#INFO failed, BAS respond timeout.";            //ACID错误
QString error5="login_error#You are not online.";	                  //你不在线
QString error6="login_error#E2901: (Third party 1)Status_Err";           //状态错误，一般指欠费
QString error7="login_error#E2901: (Third party 1)User Locked";          //用户锁定，一般也指欠费
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏
    Start();
}

void MainWindow::Start(void)
{//开始设置界面关闭按钮，关于按钮
   ui->NAME_INPUT->setAttribute(Qt::WA_InputMethodEnabled, false);
   ui->PASSWD_INPUT->setAttribute(Qt::WA_InputMethodEnabled, false);
   QPushButton *closeButton= new QPushButton(this);//建立关闭按钮
   connect(closeButton, SIGNAL(clicked()), this, SLOT(Close()));//连接关闭信号
   closeButton->setGeometry(305,0,43,33);
   closeButton->setToolTip(tr("关闭"));//设置鼠标移至按钮上的提示信息
   closeButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/close);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/close_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/close_pressed);}");//设置关闭等按钮的样式
   QPushButton *minButton= new QPushButton(this);//建立最小化按钮
   connect(minButton, SIGNAL(clicked()), this, SLOT(Min()));//连接关闭信号
   minButton->setGeometry(262,0,43,33);
   minButton->setToolTip(tr("最小化"));//设置鼠标移至按钮上的提示信息
   minButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/min);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/min_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/min_pressed);}");//设置最小化等按钮的样式
   AboutButton= new QPushButton(this);//建立关于按钮
   AboutButton->setToolTip(tr("关于"));
   AboutButton->setGeometry(320,470,20,20);
   AboutButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/about);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/about_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/about_pressed);}");
   AdvancedButton= new QPushButton(this);//建立高级设置按钮
   AdvancedButton->setToolTip(tr("高级设置"));
   AdvancedButton->setGeometry(290,470,20,20);
   AdvancedButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/advanced);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/advanced_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/advanced_pressed);}");
   AboutButton->hide();//在获取公告前隐藏这两个按钮
   AdvancedButton->hide();
   QFile open("server.json");
      if(open.open(QIODevice::ReadOnly))
      {//打开服务器信息文件
          QByteArray OPEN_CONFIG=open.readAll();
          open.close();
          QJsonDocument config=QJsonDocument::fromJson(OPEN_CONFIG);
          if(config.isObject())
          {//存在的话读入
              bool ok;
              QJsonObject obj=config.object();
              if(obj.contains("login_server"))
                 {
                  login_server=(QString)obj.value("login_server").toString();
                  ui->login_server->setText(obj.value("login_server").toString());
                }
              if(obj.contains("service_server"))
                 {
                  service_server=(QString)obj.value("service_server").toString();
                  ui->service_server->setText(obj.value("service_server").toString());  
                }
              if(obj.contains("mac"))
                 {
                  mac=(QString)obj.value("mac").toString();
                  ui->mac->setText(obj.value("mac").toString());
                }
              if(obj.contains("login_port"))
                 {
                  login_port=(QString)obj.value("login_port").toString();
                  ui->login_port->setValue(login_port.toInt(&ok,10));
                }
              if(obj.contains("service_port"))
                 {
                  service_port=(QString)obj.value("service_port").toString();
                  ui->service_port->setValue(service_port.toInt(&ok,10));
                }
              if(obj.contains("acid"))
                 {
                  acid=(QString)obj.value("acid").toString();
                  ui->acid->setValue(acid.toInt(&ok,10));
                }
              if(obj.contains("type"))
                 {
                  type=(QString)obj.value("type").toString();
                  ui->type->setValue(type.toInt(&ok,10));
                }
              if(obj.contains("drop"))
                 {
                  drop=(QString)obj.value("drop").toString();
                  ui->drop->setValue(drop.toInt(&ok,10));
                }
              if(obj.contains("pop"))
                 {
                  pop=(QString)obj.value("pop").toString();
                  ui->pop->setValue(pop.toInt(&ok,10));
                }
          }
          GetServerInfo();//获取服务器信息
      }
   else
    {//不存在服务器信息文件，要求设置
          ui->advanced_save->setGeometry(30,330,290,50);
          ui->login_server->setText("http://172.16.154.130");
          ui->service_server->setText("http://172.16.154.130");
          ui->mac->setText("02:00:00:00:00:00");
          ui->login_port->setValue(69);
          ui->service_port->setValue(8800);
          ui->acid->setValue(1);
          ui->type->setValue(3);
          ui->drop->setValue(0);
          ui->pop->setValue(1);//上面都是设置初始化值
          ui->advanced_back->hide();//只允许保存，不允许返回
          ui->stackedWidget->setCurrentIndex(4);//初始化界面
     }

}

void MainWindow::GetServerInfo(void)
{
    QNetworkAccessManager *GetServerMessageManager = new QNetworkAccessManager(this);
    QString url=login_server+"/get_msg.php";
    GetServerMessageManager->get(QNetworkRequest(QUrl(url)));
    connect(GetServerMessageManager, &QNetworkAccessManager::finished,[this](QNetworkReply *reply){
           if (reply->error() == QNetworkReply::NoError)
                {//得到信息
                    /*自动读取用户状态*/
                      QNetworkAccessManager *GetINFOManager = new QNetworkAccessManager(this);
                      GetINFOManager->get(QNetworkRequest(QUrl(login_server+"/cgi-bin/rad_user_info")));
                      connect(GetINFOManager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
                    /*自动读取用户信息*/
                     QFile open("config.json");
                        if(open.open(QIODevice::ReadOnly))
                        {
                            file_state=1;
                            QByteArray OPEN_INFO=open.readAll();
                            open.close();
                            QJsonDocument INFO=QJsonDocument::fromJson(OPEN_INFO);
                            if(INFO.isObject())
                            {
                                QJsonObject obj=INFO.object();
                                if(obj.contains("username"))
                                   { ui->NAME_INPUT->setText(obj.value("username").toString());}
                                if(obj.contains("password"))    
                                   {

                                   QString PASSWD=obj.value("password").toString();
                                    ui->PASSWD_INPUT->setText(QByteArray::fromBase64(PASSWD.toLatin1()));
                                }
                                if(obj.contains("auto_start"))
                                {
                                    bool auto_start=obj.value("auto_start").toBool();
                                   if(auto_start)
                                     ui->AUTO_START->setCheckState(Qt::Checked);
                                }
                                if(obj.contains("auto_login"))
                                {
                                    bool auto_login=obj.value("auto_login").toBool();
                                   if(auto_login)
                                    {
                                         ui->AUTO_LOGIN->setCheckState(Qt::Checked);
                                   }
                                }

                            }
                        }
                      /*对这次公告进行转码*/
                      QTextCodec *codec =QTextCodec::codecForName("GB2312");
                       QString all = codec->toUnicode(reply->readAll());
                       ui->Message_show->setText(all);
                       /*自动读取上次公告*/
                       QFile m("lastservermessage.txt");
                       if(m.open(QIODevice::ReadWrite))
                       {//读取上次公告文件
                           QString ServerMessage=m.readAll();
                           if(ServerMessage!=all)
                           {//服务器公告更新
                               ui->ShowServerMessage->setText("显示(新)公告");
                               /*自动重写公告*/
                               QFile save("lastservermessage.txt");
                               m.write(all.toUtf8());
                               m.close();
                           }
                       }
                       else
                       {//如果读取不到上次公告文件
                           ui->ShowServerMessage->setText("显示(新)公告");
                           /*自动重写公告*/
                           m.write(all.toUtf8());
                           m.close();
                       }

               }
             else
                {
                    ui->ShowState->setText("网络错误!");
                    ui->stackedWidget->setCurrentIndex(0);
                }
           connect(AboutButton, SIGNAL(clicked()), this, SLOT(on_ABOUT_clicked()));//连接信号
           connect(AdvancedButton, SIGNAL(clicked()), this, SLOT(on_ADVANCED_clicked()));//连接信号
           AboutButton->show();//在获取公告后显示这两个按钮
           AdvancedButton->show();
           reply->deleteLater();//回收
     });
}
void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{
    static int run=0;
    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf("not_online")!=-1)
        {//如果检测不在线
            state=0;
            ui->stackedWidget->setCurrentIndex(2);
            setTabOrder(ui->NAME_INPUT,ui->PASSWD_INPUT);
            setTabOrder(ui->PASSWD_INPUT,ui->NAME_INPUT);
            if(ui->AUTO_LOGIN->isChecked())
             {
                 QTimer::singleShot(3000,[this](){ui->LoginButton->click();});
             }
        }
        else
        {
            state=1;
            ui->stackedWidget->setCurrentIndex(3);
            QStringList getinfo=all.split(",");
            unsigned int login_time;
            unsigned int server_time;
            yourname=QString(getinfo.at(0));
           ui->NAME->setText(("用户名："+QString(getinfo.at(0))));
            QString temp1=getinfo.at(1);
            login_time=temp1.toInt();
            QString temp2=getinfo.at(2);
            server_time=temp2.toInt();
            QString temp3=getinfo.at(6);
            double data=temp3.toDouble()/1073741824;
            ui->DATA->setText(("流   量 ："+QString::number(data,'g',6)+"GB"));
            QString temp4=getinfo.at(7);
            usedtime=temp4.toInt()+server_time-login_time;
            ui->IP->setText(("地   址  : "+QString(getinfo.at(8))));
            reply->deleteLater();//回收
            if(run==0)
            {//第一次运行
                meTimer=new QTimer(this);  //this说明是当前类对象的定时器
                connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            }
            else
            {//以后每一次运行
                meTimer->stop();
                disconnect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));//断开信号
                connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            }
            meTimer->start(1000);
            run++;
            ui->LoginButton->setEnabled(true);
     }
    reply->deleteLater();//回收
    }
}
void MainWindow::TimeSlot()
{
//   ui->TIME_H->setText(QString::number(usedtime/3600,10));
//   ui->TIME_M->setText(QString::number(usedtime/60%60,10));
//   ui->TIME_S->setText(QString::number(usedtime%60,10));
   ui->TIME->setText(("时   间 ："+QString::number(usedtime/3600,10)+"小时"+QString::number(usedtime/60%60,10)+"分"+QString::number(usedtime%60,10)+"秒"));
   usedtime++;
}

//void MainWindow::GET_ACID_Finished(QNetworkReply *reply)
//{
//    if (reply->error() == QNetworkReply::NoError)
//    {
//        QString ac_id=reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().fileName();
//        QRegExp p("index_(\\d+).html");
//        int pos=ac_id.indexOf(p);
//        if(pos>=0)
//        acid= QString(p.cap(1));
//    }
//   reply->deleteLater();//回收
//}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Close()
{
    this->close();
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
       createActions();
       createMenu();
       mSysTrayIcon->show();
}

void MainWindow::createActions()
{
    mShowMainAction = new QAction("显示主界面");
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mExitAppAction = new QAction("退出");
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    mServiceAction = new QAction("自服务");
    connect(mServiceAction,SIGNAL(triggered()),this,SLOT(on_SERVICE_clicked()));
    mAboutAction = new QAction("关于");
    connect(mAboutAction,SIGNAL(triggered()),this,SLOT(on_aboutAppAction()));
}

void MainWindow::on_aboutAppAction()
{//按关于按钮
     ui->stackedWidget->setCurrentIndex(5);
    this->show();
    mSysTrayIcon->deleteLater();
}

void MainWindow::createMenu()
{
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
}

void MainWindow::on_showMainAction()
{//按下显示主界面
    this->show();
    mSysTrayIcon->deleteLater();
}

void MainWindow::on_exitAppAction()
{//按下退出程序
    exit(0);
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
        mSysTrayIcon->deleteLater();//一定加上删除图标
        this->show();
        break;
    default:
        break;
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{//窗口拖动，参考http://blog.csdn.net/aqtata/article/details/8902889
    if (ReleaseCapture())
        SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}


void MainWindow::on_ABOUT_clicked()
{//关于按钮
   ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_SERVICE_clicked()
{//自服务按钮
    QString url=service_server+":"+service_port;
   // qDebug()<<url;
   QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_ADVANCED_clicked()
{//高级设置按钮
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_AUTO_START_clicked()
{
    set=1;
     QSettings *reg=new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(ui->AUTO_START->isChecked())
    {
        reg->setValue("srun3k",QApplication::applicationFilePath().replace("/", "\\"));
    }
    else
    {

        reg->setValue("srun3k","");
    }
}

void MainWindow::on_AUTO_LOGIN_clicked()
{
    set=1;
}

void MainWindow::on_RetryButton_clicked()
{//重试按钮2s延时
    QTimer::singleShot(2000, this, SLOT(GetServerInfo()));
}

void MainWindow::on_LogoutButton_clicked()
{//按下注销按钮
    ui->LogoutButton->setEnabled(false);
    ui->ShowState->setText("注销中...");
       QString post="&action=logout&ac_id=";
       QNetworkAccessManager *LogoutManger = new QNetworkAccessManager(this);
       QByteArray POST;
       QNetworkRequest request;
       POST.append(post);
       POST.append(acid);
       POST.append("&mac=");
       POST.append(mac);
       POST.append("&type=");
       POST.append(type);
       POST.append("&username=");
       POST.append(yourname);
       request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
       request.setUrl(QUrl(login_server+":"+login_port+"/cgi-bin/srun_portal"));
       LogoutManger->post(request,POST);
       connect(LogoutManger, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGOUT_Finished(QNetworkReply*)));
}

void MainWindow::POST_LOGOUT_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf(logout_ok)!=-1)
        {
            QTimer::singleShot(1000,[this](){state=0;ui->ShowState->setText("注销中......成功!");});
            QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(2);ui->LogoutButton->setEnabled(true);});
         }
       else if(all.indexOf(error5)!=-1)
        {
            QTimer::singleShot(1000,[this](){state=0;ui->ShowState->setText("您不在线，无法完成注销!");});
            QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(2);ui->LogoutButton->setEnabled(true);});
         }
       else if(all.indexOf(error4)!=-1)
         {
            QTimer::singleShot(1000,[this](){ui->ShowState->setText("注销中......ACID错误!请更改高级设置中的ACID值后重试!");});
            QTimer::singleShot(3000,[this](){ui->LogoutButton->setEnabled(true);});
        }
       else if(all.indexOf(error1)!=-1)
        {
            QTimer::singleShot(1000,[this](){ui->ShowState->setText("注销中......错误!请重试!");});
            QTimer::singleShot(3000,[this](){ui->LogoutButton->setEnabled(true);});
        }
    }
    else
    {
        QTimer::singleShot(1000,[this](){ui->ShowState->setText("注销中......网络错误!请重试!");});
        QTimer::singleShot(3000,[this](){ui->LogoutButton->setEnabled(true);});
    }
   reply->deleteLater();//回收
}

void MainWindow::on_LoginButton_clicked()
{//按下登陆按钮
    ui->LoginButton->setEnabled(false);
    ui->ShowState->setText("登陆中...");
        QByteArray NAME_INPUT = ui->NAME_INPUT->text().trimmed().toLatin1();
        QByteArray PASSWD_INPUT = ui->PASSWD_INPUT->text().toLatin1();
        if(NAME_INPUT.isEmpty()||PASSWD_INPUT.isEmpty()||(NAME_INPUT.isEmpty()&&PASSWD_INPUT.isEmpty()))
        {
            ui->ShowState->setText("错误!请输入您的用户名和密码登陆!");
            ui->LoginButton->setEnabled(true);
        }
        else
        {
            char *name = NAME_INPUT.data();
            char *password = PASSWD_INPUT.data();
            QByteArray NAME_ENCRYPT="";
            for (;*name!='\0';name++)
            {//用户名加密
                NAME_ENCRYPT.append(QString(*name+4));
            }
            QByteArray PASSWD_ENCRYPT="";
            char key[]= "1234567890";
            for (int i = 0; *(password+i)!='\0'; ++i)
            {//这是密码加密函数
                 int ki = *(password+i) ^ key[strlen(key) - i%strlen(key) - 1];
                 QString _l =  (QChar)((ki & 0x0f) + 0x36);
                 QString _h =  (QChar)((ki >> 4 & 0x0f) + 0x63);
                 if (i % 2 == 0)
                 {
                     PASSWD_ENCRYPT.append(_l);
                     PASSWD_ENCRYPT.append(_h);
                 }
                 else
                 {
                     PASSWD_ENCRYPT.append(_h);
                     PASSWD_ENCRYPT.append(_l);
                 }
            }
            QNetworkAccessManager *LoginManger = new QNetworkAccessManager(this);
            QString post="&action=login&n=117&mbytes=0&minutes=0&drop=";
            QByteArray POST;
            QNetworkRequest request;
            POST.append(post);
            POST.append(drop);
            POST.append("&pop=");
            POST.append(pop);
            POST.append("&type=");
            POST.append(type);
            POST.append("&mac=");
            POST.append(mac);
            POST.append("&ac_id=");
            POST.append(acid);
            POST.append("&username=%7BSRUN3%7D%0D%0A");
            POST.append(NAME_ENCRYPT.toPercentEncoding());
            POST.append("&password=");
            POST.append(PASSWD_ENCRYPT.toPercentEncoding());
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            request.setUrl(QUrl(login_server+":"+login_port+"/cgi-bin/srun_portal"));
            LoginManger->post(request,POST);
            connect(LoginManger, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGIN_Finished(QNetworkReply*)));
            if((file_state==0||file_state==-1)||set==1)
            {
                QJsonObject info;
                 info.insert("username",QString(NAME_INPUT));
                 info.insert("password",QString(PASSWD_INPUT.toBase64()));
                  bool auto_login=ui->AUTO_LOGIN->isChecked();
                 info.insert("auto_login",auto_login);
                 bool auto_start=ui->AUTO_START->isChecked();
                 info.insert("auto_start",auto_start);
                 QJsonDocument SAVE_INFO;
                 SAVE_INFO.setObject(info);
                 QFile save("config.json");
                 if(!save.open(QIODevice::WriteOnly))
                  {
                      ui->ShowState->setText("错误!文件保存失败!");
                 }
                 else
                 {
                    save.write(SAVE_INFO.toJson());
                 }
                 save.close();
            }
        }
}

void MainWindow::POST_LOGIN_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf(login_ok_short)!=-1)
        {
            QTimer::singleShot(1000,[this](){
                state=1;
                QNetworkAccessManager *GetINFOManager = new QNetworkAccessManager(this);
                 GetINFOManager->get(QNetworkRequest(QUrl(login_server+"/cgi-bin/rad_user_info")));
                 connect(GetINFOManager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
                ui->ShowState->setText("登陆中......成功!");});
            QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(3);ui->LoginButton->setEnabled(true);});
         }
        else if((all.indexOf(error6)!=-1)||(all.indexOf(error7)!=-1))
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("您已欠费，无法使用，请充值!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error2)!=-1)
        {
                    file_state=-1;//密码错误或者用户名错误就会重写储存登陆信息
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("密码错误，请检查输入后重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error3)!=-1)
        {
                    file_state=-1;//密码错误或者用户名错误就会重写储存登陆信息
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("用户名错误，请检查输入后重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error4)!=-1)
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("登陆中......ACID错误!请更改高级设置中的ACID值后重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error1)!=-1)
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("登陆中......错误!请重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("登陆中......错误!请重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }

    }
    else
    {
        QTimer::singleShot(1000,[this](){ui->ShowState->setText("登陆中......网络错误!请重试!");});
        QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
    }
   reply->deleteLater();//回收
}

void MainWindow::on_Enter_clicked()
{//按下显示服务器公告的确定按钮
    ui->stackedWidget->setCurrentIndex(3);//返回登陆成功页面
}

void MainWindow::on_SERVICE_2_clicked()
{//注销界面的自服务按钮
    QString url=service_server+":"+service_port;
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_advanced_save_clicked()
{//按下高级界面的存储按钮
    QJsonObject config;
     config.insert("login_server",QString(ui->login_server->text().trimmed().toLatin1()));
     login_server=QString(ui->login_server->text().trimmed().toLatin1());
     config.insert("service_server",QString(ui->service_server->text().trimmed().toLatin1()));
     service_server=QString(ui->service_server->text().trimmed().toLatin1());
     config.insert("mac",QString(ui->mac->text().trimmed().toLatin1()));
     mac=QString(ui->mac->text().trimmed().toLatin1());
     config.insert("service_port",QString(ui->service_port->text()));
     service_port=QString(ui->service_port->text());
     config.insert("login_port",QString(ui->login_port->text()));
     login_port=QString(ui->login_port->text());
     config.insert("acid",QString(ui->acid->text()));
     acid=QString(ui->acid->text());
     config.insert("type",QString(ui->type->text()));
     type=QString(ui->type->text());
     config.insert("drop",QString(ui->drop->text()));
     drop=QString(ui->drop->text());
     config.insert("pop",QString(ui->pop->text()));
     pop=QString(ui->pop->text());
     QJsonDocument SAVE_CONFIG;
     SAVE_CONFIG.setObject(config);
     QFile save("server.json");
     if(!save.open(QIODevice::WriteOnly))
      {
          ui->ShowState->setText("错误!文件保存失败!");
     }
     else
     {
         ui->ShowState->setText("配置信息已经保存!");
        save.write(SAVE_CONFIG.toJson());
     }
     save.close();
//     AboutButton->hide();//在获取公告前隐藏这两个按钮
//     AdvancedButton->hide();
//     ui->Enter->setEnabled(false);
//      ui->stackedWidget->setCurrentIndex(1);
//      ui->Message_show->setText("请等待程序重新获取公告!");
      GetServerInfo();
      ui->advanced_back->show();
      ui->advanced_save->setGeometry(30,330,170,50);

}

void MainWindow::on_setdefaults_clicked()
{//按下高级界面的设置初始值
    ui->login_server->setText("http://172.16.154.130");
    ui->service_server->setText("http://172.16.154.130");
    ui->mac->setText("02:00:00:00:00:00");
    ui->login_port->setValue(69);
    ui->service_port->setValue(8800);
    ui->acid->setValue(1);
    ui->type->setValue(3);
    ui->drop->setValue(0);
    ui->pop->setValue(1);
}

void MainWindow::on_Enter_2_clicked()
{//按下关于界面的确定按钮
    if(state==0)
       {
            ui->stackedWidget->setCurrentIndex(2);
            ui->LoginButton->setEnabled(true);
       }
    else if(state==1)
    {
        ui->stackedWidget->setCurrentIndex(3);
        ui->LogoutButton->setEnabled(true);
     }
}

void MainWindow::on_advanced_back_clicked()
{//按下高级界面返回按钮
    if(state==0)
       {
             ui->stackedWidget->setCurrentIndex(2);
            ui->LoginButton->setEnabled(true);
       }
    else if(state==1)
    {
        ui->stackedWidget->setCurrentIndex(3);
        ui->LogoutButton->setEnabled(true);
     }
}

void MainWindow::on_ShowServerMessage_clicked()
{
      ui->stackedWidget->setCurrentIndex(1);
}

