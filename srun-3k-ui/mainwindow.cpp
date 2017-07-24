#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timeout.h"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QMessageBox"
#include "QTextCodec"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"
#include "QTimer"
#include "QDesktopServices"
#include "QUrl"
#include "QRegExp"
#include "QSettings"
#include "QNetworkInterface"

unsigned int usedtime;
QString yourname;
QString acid;
QString mac;
QString POSTURL= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;
int set=0;
//int heartbeat_num=0;

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
    /*自动读取配置文件*/
    ui->stackedWidget->setCurrentIndex(0);
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
   /*自动获取ACID*/
   manager = new QNetworkAccessManager(this);
   QNetworkReply *ACIDReply=manager->get(QNetworkRequest(QUrl("http://172.16.154.130")));
   QReplyTimeout *ACIDTimeout = new QReplyTimeout(ACIDReply, 1000);
   connect(ACIDTimeout, &QReplyTimeout::timeout, [=]() {
            QMessageBox::critical(NULL,"错误!","无法连接服务器!程序将退出!");
            this->close();
        });
   connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_ACID_Finished(QNetworkReply*)));

}

void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf("not_online")!=-1)
        {
            ui->stackedWidget->setCurrentIndex(0);
            ui->LOGIN->setEnabled(true);
            ui->LOGOUT->setEnabled(false);
            if(ui->AUTO_LOGIN->isChecked())
            {
                ui->LOGIN->click();
            }
        }
        else
        {
            QStringList getinfo=all.split(",");
            ui->stackedWidget->setCurrentIndex(1);
            ui->LOGIN->setEnabled(false);
            ui->LOGOUT->setEnabled(true);
            unsigned int login_time;
            unsigned int server_time;
            yourname=QString(getinfo.at(0));
            ui->NAME->setText(QString(getinfo.at(0)));
            QString temp1=getinfo.at(1);
            login_time=temp1.toInt();
            QString temp2=getinfo.at(2);
            server_time=temp2.toInt();
            QString temp3=getinfo.at(6);
            double data=temp3.toDouble()/1073741824;
            ui->DATA->setText(QString::number(data,'g',6));
            QString temp4=getinfo.at(7);
            usedtime=temp4.toInt()+server_time-login_time;
            ui->IP->setText(QString(getinfo.at(8)));
            reply->deleteLater();//回收
            meTimer=new QTimer(this);  //this说明是当前类对象的定时器
            connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            meTimer->start(1000);
            //获取物理MAC地址
            QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
            int nCnt = nets.count();
            QString strMacAddr = NULL;
            for(int i = 0; i < nCnt; i ++)
            {
               strMacAddr = nets[i].hardwareAddress();
               if((nets[i].humanReadableName().indexOf("以太网")||nets[i].humanReadableName().indexOf("本地连接"))&&(!strMacAddr.isNull()))
               {//连接名是以太网或本地连接则认为是有线网络
                   mac=strMacAddr;
//                   UdpTimer=new QTimer(this);  //this说明是当前类对象的定时器
//                   connect(UdpTimer,SIGNAL(timeout()),this,SLOT(UdpSlot()));  //把信号与槽进行连接
//                   UdpTimer->start(30000);//30s发送一次udp广播
               }
               else
               {
                  //没有找到本地连接
                   QMessageBox::critical(NULL,"错误!","找不到本地连接");
               }
            }
        }
     }
     else
     {//得不到信息
         QMessageBox::critical(NULL,"错误!","无法连接服务器!");
     }
    reply->deleteLater();//回收
}

void MainWindow::TimeSlot()
{
   ui->TIME_H->setText(QString::number(usedtime/3600,10));
   ui->TIME_M->setText(QString::number(usedtime/60%60,10));
   ui->TIME_S->setText(QString::number(usedtime%60,10));
   usedtime++;
}

//UDP心跳包，官方已更改
//void MainWindow::UdpSlot()
//{
//    QString UdpData="! 12s 20x 17s 7x"+yourname+mac;
//    sender = new QUdpSocket(this);
//    sender->writeDatagram(UdpData,UdpData.length(),"172.16.154.130",3338);
//    heartbeat_num++;
//    ui->HEARTBEAT->setText(QString::number(heartbeat_num,10));
//    delete sender;
//}

MainWindow::~MainWindow()
{
    delete manager;
    delete meTimer;
    delete ui;
}

void MainWindow::on_SERVICE_clicked()
{
     QDesktopServices::openUrl(QUrl("http://172.16.154.130:8800"));
}

void MainWindow::on_GET_MESSAGE_clicked()
{
    ui->stackedWidget->setEnabled(false);
    QNetworkReply *pReply=manager->get(QNetworkRequest(QUrl("http://172.16.154.130/get_msg.php")));
    QReplyTimeout *pTimeout = new QReplyTimeout(pReply, 1000);
    connect(pTimeout, &QReplyTimeout::timeout, [=]() {
          QMessageBox::critical(NULL,"错误!","无法连接服务器!");
      });
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_MESSAGE_Finished(QNetworkReply*)));
    ui->stackedWidget->setEnabled(true);
}

void MainWindow::GET_MESSAGE_Finished(QNetworkReply *reply)
{

    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        QMessageBox::information(this, tr(":) 公告信息"),all);
     }
    reply->deleteLater();//回收
}

void MainWindow::on_LOGOUT_clicked()
{
   ui->stackedWidget->setEnabled(false);
   QString post="&mac=&type=2&action=logout&ac_id=";
   manager = new QNetworkAccessManager(this);
   QByteArray POST;
   QNetworkRequest request;
   POST.append(post);
   POST.append(acid);
   POST.append("&username=");
   POST.append(yourname);
   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
   request.setUrl(QUrl(POSTURL));
   QNetworkReply *LogoutReply=manager->post(request,POST);
   QReplyTimeout *LogoutTimeout = new QReplyTimeout(LogoutReply, 1000);
   connect(LogoutTimeout, &QReplyTimeout::timeout, [=]() {
            QMessageBox::critical(NULL,"错误!","无法连接服务器!");
        });
   connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGOUT_Finished(QNetworkReply*)));
   ui->stackedWidget->setEnabled(true);
}

void MainWindow::POST_LOGOUT_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf(logout_ok)!=-1)
        {
              QMessageBox::information(this, tr(":) 注销成功!"),tr("您已注销成功!"));
              ui->stackedWidget->setCurrentIndex(0);
              ui->LOGIN->setEnabled(true);
              ui->LOGOUT->setEnabled(false);
         }
       else if(all.indexOf(error5)!=-1)
           QMessageBox::critical(this,tr(":( 注销失败!"),tr("您不在线，无法完成注销!"));
       else if(all.indexOf(error4)!=-1)
           QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
       else if(all.indexOf(error1)!=-1)
           ui->LOGOUT->click();
    }
   reply->deleteLater();//回收
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

void MainWindow::on_LOGIN_clicked()
{
    ui->stackedWidget->setEnabled(false);
    QByteArray NAME_INPUT = ui->NAME_INPUT->text().trimmed().toLatin1();
    QByteArray PASSWD_INPUT = ui->PASSWD_INPUT->text().toLatin1();
    if(NAME_INPUT.isEmpty()||PASSWD_INPUT.isEmpty()||(NAME_INPUT.isEmpty()&&PASSWD_INPUT.isEmpty()))
    {
        QMessageBox::critical(this, tr("错误!"),tr("请输入您的用户名和密码登陆"));
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
        QString post="&action=login&drop=0&pop=1&type=10&n=117&mbytes=0&minutes=0&mac=";
        QByteArray POST;
        QNetworkRequest request;
        POST.append(post);
        POST.append(mac);
        POST.append("&ac_id=");
        POST.append(acid);
        POST.append("&username=%7BSRUN3%7D%0D%0A");
        POST.append(NAME_ENCRYPT.toPercentEncoding());
        POST.append("&password=");
        POST.append(PASSWD_ENCRYPT.toPercentEncoding());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setUrl(QUrl(POSTURL));
        QNetworkReply *LoginReply=manager->post(request,POST);
        QReplyTimeout *LoginTimeout = new QReplyTimeout(LoginReply, 1000);
        connect(LoginTimeout, &QReplyTimeout::timeout, [=]() {
                 QMessageBox::critical(NULL,"错误!","无法连接服务器!");
             });
        connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGIN_Finished(QNetworkReply*)));
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
                 QMessageBox::critical(this, tr("错误!"),tr("文件保存失败!"));
             }
             else
             {
                save.write(SAVE_INFO.toJson());
             }
             save.close();
        }
    }
    ui->stackedWidget->setEnabled(true);
}

void MainWindow::POST_LOGIN_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        if(all.indexOf(login_ok_short)!=-1)
        {
            QMessageBox::information(this, tr(":) 登陆成功!"),tr("您已登陆成功!"));
            connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
            manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
         }
        else if((all.indexOf(error6)!=-1)||(all.indexOf(error7)!=-1))
                QMessageBox::critical(this,tr(":( 欠费无法使用!"),tr("你已欠费，无法使用，请充值!"));
        else if(all.indexOf(error2)!=-1)
                QMessageBox::critical(this,tr(":( 密码错误!"),tr("密码错误，请检查输入后重试!"));
        else if(all.indexOf(error3)!=-1)
                QMessageBox::critical(this,tr(":( 用户名错误!"),tr("用户名错误，请检查输入后重试!"));
        else if(all.indexOf(error4)!=-1)
                QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
        else if(all.indexOf(error1)!=-1)
                ui->LOGIN->click();
        else
                QMessageBox::critical(this,tr(":( 其他错误!"),QString(all));

    }
   reply->deleteLater();//回收
}

void MainWindow::GET_ACID_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QString ac_id=reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().fileName();
        QRegExp p("index_(\\d+).html");
        int pos=ac_id.indexOf(p);
        if(pos>=0)
        acid= QString(p.cap(1));
    }
   reply->deleteLater();//回收
   /*自动获取状态*/
   connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
   manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
}
