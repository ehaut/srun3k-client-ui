#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QMessageBox"
#include "QFile"
#include "QSettings"
#include "QTextCodec"
#include "string"
/*使用Qt网络完成*/
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"

QString POSTURL= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;
int set=0;

typedef struct feedback_info
{
    char name[20];
    char data[20];
    char time[20];
    char ip[20];
}info;


struct MemoryStruct {
  char *memory;
  size_t size;
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
                  { ui->INPUT_NAME->setText(obj.value("username").toString());}
               if(obj.contains("password"))
                  {ui->INPUT_PASSWD->setText(obj.value("password").toString());}
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
                       ui->LOGIN->click();
                  }
               }

           }
       }
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LOGIN_clicked()
{
    QByteArray NAME_INPUT = ui->INPUT_NAME->text().trimmed().toLatin1();
    QByteArray PASSWD_INPUT = ui->INPUT_PASSWD->text().toLatin1();
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

void MainWindow::on_LOGOUT_clicked()
{
    QString NAME_INPUT = ui->INPUT_NAME->text().trimmed();
    QString PASSWD_INPUT = ui->INPUT_PASSWD->text();
    if(NAME_INPUT.isEmpty())//如果用户名是空的
     {
        QMessageBox::critical(this, tr("错误!"),tr("请输入您的用户名注销"));
    }
    if(!(NAME_INPUT.isEmpty()))
     {
           QString post="&mac=&type=2&action=logout&ac_id=1&username=";
           manager = new QNetworkAccessManager(this);
           QByteArray POST;
           QNetworkRequest request;
           POST.append(post);
           POST.append(NAME_INPUT);
           request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
           request.setUrl(QUrl(POSTURL));
           connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(POST_LOGOUT_Finished(QNetworkReply*)));
           manager->post(request,POST);
     }
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


void MainWindow::on_INFO_clicked()
{
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
             QMessageBox::information(this, tr(":) 您的信息!"),tr("您不在线!"));
        else
        {
            info i;
            int t=0,j=0,k=0;
            char temp[20]={0}; //定义一个临时变量
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
                       strcpy(i.name,temp);
                   if(flag==7)
                       strcpy(i.data,temp);
                   if(flag==8)
                       strcpy(i.time,temp);
                   if(flag==9)
                   {
                       strcpy(i.ip,temp);
                       break;
                   }
                   k++;
                   continue;
               }
            }
            double data_used=atof(i.data)/1073741824;
            int time_h=atoi(i.time)/3600;
            int time_m=atoi(i.time)/60%60;
            int time_s=atoi(i.time)%60;
            QString put=+"您已在线，下面是您的登录信息:\n用户名:"+QString(i.name)+"\n"+"流量:"+ QString::number(data_used,'g',6)+"GB\n"+"时间:"+\
            QString::number(time_h,10)+"时"+ QString::number(time_m,10)+"分"+ QString::number(time_s,10)+"秒\n"+"IP:"+QString(i.ip);
            QMessageBox::information(this, tr(":) 您的信息!"),put);
     }
   }
     else
     {//得不到信息
         QMessageBox::critical(NULL,"错误!","无法连接服务器!");
     }
    reply->deleteLater();//回收
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
