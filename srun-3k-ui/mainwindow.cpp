#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include "QSettings"
unsigned int usedtime;
QString *yourname;
QString POSTURL= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;
int set=0;

//struct feedback_info *getinfo;//储存信息结构体指针

//typedef struct feedback_info
//{
//    char name[20];
//    double data;
//    long time;
//    char ip[20];
//}info;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*自动读取配置文件*/
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
    /*自动获取状态*/
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
}

void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        std::string get=all.toStdString();
        if(get[0]=='n')
        {//不在线
            ui->InfoWidget->setEnabled(false);
            ui->InputWidget->setEnabled(true);
            ui->InfoWidget->setVisible(false);
           ui->InputWidget->setVisible(true);
           ui->LOGIN->setEnabled(true);
           ui->LOGOUT->setEnabled(false);
           if(ui->AUTO_LOGIN->isChecked())
           {
                ui->LOGIN->click();
           }
        }
        else
        {//在线，将信息读取到结构体中
            ui->InfoWidget->setEnabled(true);
            ui->InputWidget->setEnabled(false);
            ui->InfoWidget->setVisible(true);
           ui->InputWidget->setVisible(false);
           ui->LOGIN->setEnabled(false);
           ui->LOGOUT->setEnabled(true);
//            getinfo=(struct feedback_info *)new info;
            int t=0,j=0,k=0;
            char temp[20]={0}; //定义一个临时变量
            unsigned int login_time;
            unsigned int server_time;
            for(int flag=0;get[t]!='\0';t++)//开始循环直到数据末尾
            {
                memset(temp,0,sizeof(temp));//将temp字符数组重置
               if(get[t]==',')//如果当前位为,分隔符
               {
                   flag++;//分隔符计数
                   for(j=0;k!=t;j++,k++)
                       temp[j]=get[k];
                   temp[j]='\0';
                   if(flag==1)
                    {
                       //strcpy(getinfo->name,temp);
                       ui->NAME->setText(QString::fromStdString(temp));
                       yourname=(QString*)new QString(QString::fromStdString(temp));

                    }
                   if(flag==2)
                   {//登录时间
                       login_time=atoi(temp);
                   }
                   if(flag==3)
                   {//服务器时间
                       server_time=atoi(temp);
                   }
                   if(flag==7)
                   {
                       //getinfo->data=atof(temp)/1073741824;
                       ui->DATA->setText(QString::number(atof(temp)/1073741824,'g',6));
                   }
                   if(flag==8)
                   {
                       //getinfo->time=atoi(temp);
//                       ui->TIME_H->setText(QString::number(atoi(temp)/3600,10));
//                       ui->TIME_M->setText(QString::number(atoi(temp)/60%60,10));
//                       ui->TIME_S->setText(QString::number(atoi(temp)%60,10));
                       usedtime=atoi(temp)+server_time-login_time;
                   }
                   if(flag==9)
                   {
                      // strcpy(getinfo->ip,temp);
                       ui->IP->setText(QString::fromStdString(temp));
                       break;
                   }
                   k++;
                   continue;
               }
            }
            reply->deleteLater();//回收
            meTimer=new QTimer(this);  //this说明是当前类对象的定时器
            connect(meTimer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            meTimer->start(1000);
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

MainWindow::~MainWindow()
{
    delete yourname;
    delete ui;
}

void MainWindow::on_SERVICE_clicked()
{
     QDesktopServices::openUrl(QUrl("http://172.16.154.130:8800"));
}

void MainWindow::on_GET_MESSAGE_clicked()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_MESSAGE_Finished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://172.16.154.130/get_msg.php")));
}

void MainWindow::GET_MESSAGE_Finished(QNetworkReply *reply)
{

    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        QMessageBox::information(this, tr(":) 公告信息"),all);
     }
     else
     {//得不到信息
         QMessageBox::critical(NULL,"错误!","无法连接服务器!");
     }
    reply->deleteLater();//回收
}

void MainWindow::on_LOGOUT_clicked()
{
   QString post="&mac=&type=2&action=logout&ac_id=1&username=";
   manager = new QNetworkAccessManager(this);
   QByteArray POST;
   QNetworkRequest request;
   POST.append(post);
   POST.append(*yourname);
   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
   request.setUrl(QUrl(POSTURL));
   connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGOUT_Finished(QNetworkReply*)));
   manager->post(request,POST);
}

void MainWindow::POST_LOGOUT_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        std::string get=all.toStdString();
        for(int i=0;get[i]!='\0';i++)
        {
           if(get[i]=='t'&&get[i+2]=='o')
                   {//注销成功情况
                      QMessageBox::information(this, tr(":) 注销成功!"),tr("您已注销成功!"));
                      ui->InfoWidget->setEnabled(false);
                      ui->InputWidget->setEnabled(true);
                      ui->InfoWidget->setVisible(false);
                     ui->InputWidget->setVisible(true);
                     ui->LOGIN->setEnabled(true);
                     ui->LOGOUT->setEnabled(false);
                      break;
                   }
           else if(get[i]=='Y'&&get[i+1]=='o')
                   {//不在线情况
                       QMessageBox::critical(this,tr(":( 注销失败!"),tr("您不在线，无法完成注销!"));
                       break;
                   }
           else if(get[i]=='I'&&get[i+1]=='N')
                   {//ACID错误情况
                       QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
                       break;
                   }
           else if(get[i]=='m')
                   {//参数错误情况
                      QMessageBox::critical(this,tr(":( 参数错误!"),tr("参数错误，请重新输入重试!"));
                       break;
                   }
       }
    }
    else
    {//得不到信息
        QMessageBox::critical(NULL,"错误!","无法连接服务器!");
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
        QString post="&action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&ac_id=1&username=%7BSRUN3%7D%0D%0A";
        manager = new QNetworkAccessManager(this);
        QByteArray POST;
        QNetworkRequest request;
        POST.append(post);
        POST.append(NAME_ENCRYPT.toPercentEncoding());
        POST.append("&password=");
        POST.append(PASSWD_ENCRYPT.toPercentEncoding());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setUrl(QUrl(POSTURL));
        connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGIN_Finished(QNetworkReply*)));
        manager->post(request,POST);
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
}

void MainWindow::POST_LOGIN_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        std::string get=all.toStdString();
        for(int i=0;get[i]!='\0';i++)
        {
            if(get[i+1]=='\0')
            {
                QMessageBox::critical(this,tr(":( 其他错误!"),QString::fromStdString(get));
                 break;
            }
            else if(get[i]=='n'&&get[i+2]=='o')
            {//登陆成功情况
                QMessageBox::information(this, tr(":) 登陆成功!"),tr("您已登陆成功!"));
                manager = new QNetworkAccessManager(this);
                connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
                manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
                break;
            }
            else if(get[i]=='P'&&get[i+1]=='a')
            {//密码错误情况
                QMessageBox::critical(this,tr(":( 密码错误!"),tr("密码错误，请检查输入后重试!"));
                break;
            }
            else if(get[i]=='U'&&get[i+1]=='s')
            {//用户名错误情况
                QMessageBox::critical(this,tr(":( 用户名错误!"),tr("用户名错误，请检查输入后重试!"));
                break;
            }
            else if(get[i]=='I'&&get[i+1]=='N')
            {//ACID错误情况
                QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
                break;
            }
            else if(get[i]=='m')
            {//参数错误情况
               QMessageBox::critical(this,tr(":( 参数错误!"),tr("参数错误，请重新输入重试!"));
                break;
            }
        }
    }
    else
    {//得不到信息
        QMessageBox::critical(NULL,"错误!","无法连接服务器!");
    }
   reply->deleteLater();//回收
}
