#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
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
#include "QDebug"
unsigned int usedtime;
QString yourname;
QString POSTURL= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;
int set=0;
int state=0;
QString acid="2";
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
    QTimer::singleShot(2000, this, SLOT(GetServerInfo()));
}

void MainWindow::Start(void)
{//开始设置界面关闭按钮，关于按钮
   QPushButton *closeButton= new QPushButton(this);//建立关闭按钮
   connect(closeButton, SIGNAL(clicked()), this, SLOT(Close()));//连接关闭信号
   closeButton->setGeometry(305,0,43,33);
   closeButton->setToolTip(tr("关闭"));//设置鼠标移至按钮上的提示信息
   closeButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/close);}"
                              "QPushButton:hover {border-image: url(:/titleButtons/close_hover);}"
                               "QPushButton:pressed {border-image: url(:/titleButtons/close_pressed);}");//设置关闭等按钮的样式
   QPushButton *AboutButton= new QPushButton(this);//建立关闭按钮
   connect(AboutButton, SIGNAL(clicked()), this, SLOT(on_ABOUT_clicked()));//连接信号
   AboutButton->setToolTip(tr("关于"));
   AboutButton->setGeometry(340,490,10,10);
   AboutButton->setStyleSheet("QPushButton {border-image: url(:/titleButtons/about);}");
   ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::GetServerInfo(void)
{//得到服务器公告和acid值
    ui->Enter->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(1);
    ui->ShowState->setText("获取服务器公告中...");
    QNetworkAccessManager *GetServerMessageManager = new QNetworkAccessManager(this);
    GetServerMessageManager->get(QNetworkRequest(QUrl("http://172.16.154.130/get_msg.php")));
    connect(GetServerMessageManager, &QNetworkAccessManager::finished,[this](QNetworkReply *reply){
           if (reply->error() == QNetworkReply::NoError)
                {//得到信息
                 /*自动读取ACID值*/
//                 QNetworkAccessManager *GetACIDManager = new QNetworkAccessManager(this);
//                  GetACIDManager->get(QNetworkRequest(QUrl("http://172.16.154.130")));
//                  connect(GetACIDManager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_ACID_Finished(QNetworkReply*)));
                 /*自动读取用户状态*/
                  QNetworkAccessManager *GetINFOManager = new QNetworkAccessManager(this);
                   GetINFOManager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
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
                                {ui->PASSWD_INPUT->setText(obj.value("password").toString());}
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
                   QTimer::singleShot(1000,[this](){ui->ShowState->setText("获取服务器公告中...成功!");});
                   QTextCodec *codec =QTextCodec::codecForName("GB2312");
                   QString all = codec->toUnicode(reply->readAll());
                   QTimer::singleShot(1000,[this,all](){
                       ui->Message_show->setText(all);
                       ui->Enter->setEnabled(true);
                   });
                   QTimer::singleShot(10000,[this](){//10s后自动跳转
                       if(state==0)
                          {
                                ui->stackedWidget->setCurrentIndex(2);
                                if(ui->AUTO_LOGIN->isChecked())
                                {
                                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);ui->LoginButton->click();});
                                }
                                ui->LoginButton->setEnabled(true);
                          }
                       else
                       {
                           ui->stackedWidget->setCurrentIndex(3);
                           ui->LogoutButton->setEnabled(true);
                        }
                });
           }
           else
               {
                   QTimer::singleShot(1000,[this](){ui->ShowState->setText("获取服务器公告中...网络错误!");});
                   QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(0);});
               }
             reply->deleteLater();//回收
        });
}

void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf("not_online")!=-1)
        {
            state=0;
        }
        else
        {
            state=1;
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
            meTimer=new QTimer(this);  //this说明是当前类对象的定时器
            connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            meTimer->start(1000);
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
    close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{//窗口拖动，参考http://blog.csdn.net/aqtata/article/details/8902889
    if (ReleaseCapture())
        SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}


void MainWindow::on_ABOUT_clicked()
{//关于按钮
    QDesktopServices::openUrl(QUrl("https://github.com/CHN-STUDENT/SRUN-3K-UI"));
}

void MainWindow::on_SERVICE_clicked()
{//自服务按钮
    QDesktopServices::openUrl(QUrl("http://172.16.154.130:8800/"));
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
{
    ui->LogoutButton->setEnabled(false);
    ui->ShowState->setText("注销中...");
       QString post="&mac=&type=2&action=logout&ac_id=";
       QNetworkAccessManager *LogoutManger = new QNetworkAccessManager(this);
       QByteArray POST;
       QNetworkRequest request;
       POST.append(post);
       POST.append(acid);
       POST.append("&username=");
       POST.append(yourname);
       request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
       request.setUrl(QUrl(POSTURL));
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
            QTimer::singleShot(1000,[this](){ui->ShowState->setText("注销中......成功!");});
            QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(2);ui->LogoutButton->setEnabled(true);});
         }
       else if(all.indexOf(error5)!=-1)
        {
            QTimer::singleShot(1000,[this](){ui->ShowState->setText("您不在线，无法完成注销!");});
            QTimer::singleShot(3000,[this](){ui->stackedWidget->setCurrentIndex(2);ui->LogoutButton->setEnabled(true);});
         }
       else if(all.indexOf(error4)!=-1)
         {
            QTimer::singleShot(1000,[this](){ui->ShowState->setText("注销中......错误!请重试!");});
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
{
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
            QString post="&action=login&drop=0&pop=1&type=10&n=117&mbytes=0&minutes=0&mac=";
            QByteArray POST;
            QNetworkRequest request;
            POST.append(post);
            POST.append("&ac_id=");
            POST.append(acid);
            POST.append("&username=%7BSRUN3%7D%0D%0A");
            POST.append(NAME_ENCRYPT.toPercentEncoding());
            POST.append("&password=");
            POST.append(PASSWD_ENCRYPT.toPercentEncoding());
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            request.setUrl(QUrl(POSTURL));
            LoginManger->post(request,POST);
            connect(LoginManger, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGIN_Finished(QNetworkReply*)));
            if(file_state==0||set==1)
            {
                QJsonObject info;
                 info.insert("username",QString(NAME_INPUT));
                 info.insert("password",QString(PASSWD_INPUT));
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
                QNetworkAccessManager *GetINFOManager = new QNetworkAccessManager(this);
                 GetINFOManager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
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
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("密码错误，请检查输入后重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error3)!=-1)
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("用户名错误，请检查输入后重试!");});
                    QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);});
         }
        else if(all.indexOf(error4)!=-1)
        {
                    QTimer::singleShot(1000,[this](){ui->ShowState->setText("登陆中......错误!请重试!");});
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
{
    if(state==0)
       {
             ui->stackedWidget->setCurrentIndex(2);
             if(ui->AUTO_LOGIN->isChecked())
             {
                 QTimer::singleShot(3000,[this](){ui->LoginButton->setEnabled(true);ui->LoginButton->click();});
             }
             ui->LoginButton->setEnabled(true);
       }
    else
    {
        ui->stackedWidget->setCurrentIndex(3);
        ui->LogoutButton->setEnabled(true);
     }
}

void MainWindow::on_SERVICE_2_clicked()
{
    QDesktopServices::openUrl(QUrl("http://172.16.154.130:8800/"));
}
