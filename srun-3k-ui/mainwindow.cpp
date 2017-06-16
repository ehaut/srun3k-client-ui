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

unsigned int usedtime;
QString *yourname;
QString POSTURL= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;
int set=0;
int state=0;
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
         ui->InfoWidget->setEnabled(true);
        if(get[0]=='n')
        {//不在线
            state=0;
            ui->InfoWidget->setEnabled(false);
            ui->InputWidget->setEnabled(true);
            ui->InfoWidget->setVisible(false);
           ui->InputWidget->setVisible(true);
           ui->pushButton->setText("登陆");
           ui->pushButton->setEnabled(true);
           ui->pushButton->setStyleSheet(" background: #0f88eb;"
                                             "border: 0;"
                                             "border-radius: 3px;"
                                             "line-height: 41px;"
                                             "color: #fff;");
           if(ui->AUTO_LOGIN->isChecked())
           {
                ui->pushButton->click();
           }
        }
        else
        {//在线
            state=1;
            ui->InputWidget->setEnabled(false);
            ui->InfoWidget->setVisible(true);
           ui->InputWidget->setVisible(false);
           ui->pushButton->setEnabled(true);
              ui->INFO->setAlignment(Qt::AlignCenter);
              ui->pushButton->setText("注销");
              ui->pushButton->setStyleSheet(" background: #FFD700;"
                                                "border: 0;"
                                                "border-radius: 3px;"
                                                "line-height: 41px;"
                                                "color: #fff;");
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
                       QString a=QString::fromStdString("用户名：");
                       QString b=QString::fromStdString(temp);
                       QString output=a+b;
                       ui->NAME->setText(output);
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
                       QString a=QString::fromStdString("流   量 ：");
                       QString b=QString::number(atof(temp)/1073741824,'g',6);
                       QString c=QString::fromStdString(" GB");
                       QString output=a+b+c;
                       ui->DATA->setText(output);
                   }
                   if(flag==8)
                   {
                       usedtime=atoi(temp)+server_time-login_time;
                   }
                   if(flag==9)
                   {
                       QString a=QString::fromStdString("地   址 ：");
                       QString b=QString::fromStdString(temp);
                       QString c=QString::fromStdString(" GB");
                       QString output=a+b;
                       ui->IP->setText(output);
                       break;
                   }
                   k++;
                   continue;
               }
            }
            reply->deleteLater();//回收
            Timer=new QTimer(this);  //this说明是当前类对象的定时器
            connect(Timer,SIGNAL(timeout()),this,SLOT(TimeSlot()));  //把信号与槽进行连接
            Timer->start(1000);
        }
     }
     else
     {//得不到信息
        ui->InfoWidget->setEnabled(false);
        ui->InputWidget->setEnabled(false);
        ui->InfoWidget->setVisible(false);
       ui->InputWidget->setVisible(false);
        ui->pushButton->setText("获取不到信息！5s后自动重试...");
        ui->pushButton->setStyleSheet(" background: #ff0000;"
                                          "border: 0;"
                                          "border-radius: 3px;"
                                          "line-height: 41px;"
                                          "color: #fff;");
        RetryTimer=new QTimer(this);  //this说明是当前类对象的定时器
        connect(RetryTimer,SIGNAL(timeout()),this,SLOT(GetTryTimeSlot()));  //把信号与槽进行连接
       RetryTimer->setSingleShot(true);//单次触发
        RetryTimer->start(5000);
     }
    reply->deleteLater();//回收
}


void MainWindow::GetTryTimeSlot()
{
    ui->pushButton->setText("重试中...");
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
}

void MainWindow::PostTryTimeSlot()
{
   ui->pushButton->setText("重试中...");
   ui->pushButton->setEnabled(true);
   ui->pushButton->click();
}

void MainWindow::TimeSlot()
{
   int time_h=usedtime/3600;
   int time_m=usedtime/60%60;
   int time_s=usedtime%60;
   QString output=QString::fromStdString("时   间 ：")+ QString::number(time_h,10)+"时"+ QString::number(time_m,10)+"分"+ QString::number(time_s,10)+"秒";
   ui->TIME->setText(output);
   usedtime++;
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
    ui->pushButton->setEnabled(false);
    if(state==0)
    {
        QByteArray NAME_INPUT = ui->NAME_INPUT->text().trimmed().toLatin1();
        QByteArray PASSWD_INPUT = ui->PASSWD_INPUT->text().toLatin1();
        if(NAME_INPUT.isEmpty()||PASSWD_INPUT.isEmpty()||(NAME_INPUT.isEmpty()&&PASSWD_INPUT.isEmpty()))
        {
            ui->pushButton->setText("错误：请输入用户名和密码！");
           ui->pushButton->setStyleSheet(" background:#ff0000;"
                                         "border: 0;"
                                         "border-radius: 3px;"
                                         "line-height: 41px;"
                                         "color: #fff;");
             RetryTimer=new QTimer(this);  //this说明是当前类对象的定时器
             connect(RetryTimer,SIGNAL(timeout()),this,SLOT(ReLoginTimeSlot()));  //把信号与槽进行连接
             RetryTimer->setSingleShot(true);//单次触发
             RetryTimer->start(5000);
        }
        else
        {
              ui->pushButton->setText("登陆中...");
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
                     ui->pushButton->setText("错误：配置文件保存失败！");
                     ui->pushButton->setStyleSheet(" background: #ff0000;"
                                                       "border: 0;"
                                                       "border-radius: 3px;"
                                                       "line-height: 41px;"
                                                       "color: #fff;");
                 }
                 else
                 {
                    save.write(SAVE_INFO.toJson());
                 }
                 save.close();
            }
        }
    }
    if(state==1)
    {
        ui->pushButton->setText("注销中...");
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
}

void MainWindow::ReLoginTimeSlot()
{//登陆按钮显示等待时间
   ui->pushButton->setText("登陆");
   ui->pushButton->setStyleSheet(" background: #0f88eb;"
                                     "border: 0;"
                                     "border-radius: 3px;"
                                     "line-height: 41px;"
                                     "color: #fff;");
   ui->pushButton->setEnabled(true);
}


void MainWindow::LogoutTimeSlot()
{//注销按钮显示等待时间
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
   ui->pushButton->setText("注销");
   ui->pushButton->setStyleSheet(" background: #FFD700;"
                                     "border: 0;"
                                     "border-radius: 3px;"
                                     "line-height: 41px;"
                                     "color: #fff;");
   ui->pushButton->setEnabled(true);
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
               state=0;
               ui->InfoWidget->setEnabled(false);
               ui->InputWidget->setEnabled(true);
               ui->InfoWidget->setVisible(false);
              ui->InputWidget->setVisible(true);
                    ui->pushButton->setText("注销成功！5s后可重登录...");
                   ui->pushButton->setStyleSheet(" background: #00aa00;"
                                                 "border: 0;"
                                                 "border-radius: 3px;"
                                                 "line-height: 41px;"
                                                 "color: #fff;");
                     WaitTimer=new QTimer(this);  //this说明是当前类对象的定时器
                     connect(WaitTimer,SIGNAL(timeout()),this,SLOT(ReLoginTimeSlot()));  //把信号与槽进行连接
                     WaitTimer->setSingleShot(true);//单次触发
                     WaitTimer->start(5000);
                      break;
                   }
           else if(get[i]=='Y'&&get[i+1]=='o')
                   {//不在线情况
                     ui->pushButton->setText("您不在线！无法注销！");
                     ui->pushButton->setStyleSheet(" background: #ff0000;"
                                                 "border: 0;"
                                                 "border-radius: 3px;"
                                                 "line-height: 41px;"
                                                 "color: #fff;");
                       break;
                   }
           else if(get[i]=='I'&&get[i+1]=='N')
                   {//ACID错误情况
               ui->pushButton->setText("ACID错误！");
               ui->pushButton->setStyleSheet(" background: #ff0000;"
                                           "border: 0;"
                                           "border-radius: 3px;"
                                           "line-height: 41px;"
                                           "color: #fff;");
                       break;
                   }
           else if(get[i]=='m')
                   {//参数错误情况
               ui->pushButton->setText("参数错误！");
               ui->pushButton->setStyleSheet(" background: #ff0000;"
                                           "border: 0;"
                                           "border-radius: 3px;"
                                           "line-height: 41px;"
                                           "color: #fff;");
                       break;
                   }
       }
    }
    else
    {//得不到信息
         ui->pushButton->setText("无法连接服务器！5s后自动重试...");
         ui->pushButton->setStyleSheet(" background: #ff0000;"
                                           "border: 0;"
                                           "border-radius: 3px;"
                                           "line-height: 41px;"
                                           "color: #fff;");
         RetryTimer=new QTimer(this);  //this说明是当前类对象的定时器
         connect(Timer,SIGNAL(timeout()),this,SLOT(PostTryTimeSlot()));  //把信号与槽进行连接
         RetryTimer->setSingleShot(true);//单次触发
         RetryTimer->start(5000);
    }
   reply->deleteLater();//回收
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

               if(get[i]=='n'&&get[i+2]=='o')
               {//登陆成功情况
                   state=1;
                   ui->InfoWidget->setEnabled(true);
                   ui->InputWidget->setEnabled(false);
                   ui->InfoWidget->setVisible(true);
                  ui->InputWidget->setVisible(false);
                   ui->pushButton->setText("登陆成功！5s后可注销...");
                  ui->pushButton->setStyleSheet(" background: #00aa00;"
                                                "border: 0;"
                                                "border-radius: 3px;"
                                                "line-height: 41px;"
                                                "color: #fff;");
                  WaitTimer=new QTimer(this);  //this说明是当前类对象的定时器
                  connect(WaitTimer,SIGNAL(timeout()),this,SLOT(LogoutTimeSlot()));  //把信号与槽进行连接
                  WaitTimer->setSingleShot(true);//单次触发
                  WaitTimer->start(5000);
                   break;
               }
               else if((get[i]=='S'&&get[i+1]=='t')||(get[i]=='L'&&get[i+1]=='o'))
               {//欠费情况
                   ui->pushButton->setText("欠费无法使用！");
                   ui->pushButton->setStyleSheet(" background: #ff0000;"
                                               "border: 0;"
                                               "border-radius: 3px;"
                                               "line-height: 41px;"
                                               "color: #fff;");
                   WaitTimer=new QTimer(this);  //this说明是当前类对象的定时器
                   connect(Timer,SIGNAL(timeout()),this,SLOT(ReLoginTimeSlot()));  //把信号与槽进行连接
                   WaitTimer->setSingleShot(true);//单次触发
                   WaitTimer->start(5000);
                    break;
               }
               else if(get[i]=='P'&&get[i+1]=='a')
               {//密码错误情况
                   ui->pushButton->setText("密码错误！");
                   ui->pushButton->setStyleSheet(" background: #ff0000;"
                                               "border: 0;"
                                               "border-radius: 3px;"
                                               "line-height: 41px;"
                                               "color: #fff;");
                   WaitTimer=new QTimer(this);  //this说明是当前类对象的定时器
                   connect(WaitTimer,SIGNAL(timeout()),this,SLOT(ReLoginTimeSlot()));  //把信号与槽进行连接
                   WaitTimer->setSingleShot(true);//单次触发
                   WaitTimer->start(5000);
                   break;
               }
               else if(get[i]=='U'&&get[i+1]=='s')
               {//用户名错误情况
                   ui->pushButton->setText("用户名错误！");
                   ui->pushButton->setStyleSheet(" background: #ff0000;"
                                               "border: 0;"
                                               "border-radius: 3px;"
                                               "line-height: 41px;"
                                               "color: #fff;");
                  WaitTimer=new QTimer(this);  //this说明是当前类对象的定时器
                   connect(WaitTimer,SIGNAL(timeout()),this,SLOT(ReLoginTimeSlot()));  //把信号与槽进行连接
                  WaitTimer->setSingleShot(true);//单次触发
                   WaitTimer->start(5000);
                   break;
               }
               else if(get[i]=='I'&&get[i+1]=='N')
               {//ACID错误情况
                   ui->pushButton->setText("ACID错误！");
                   ui->pushButton->setStyleSheet(" background: #ff0000;"
                                               "border: 0;"
                                               "border-radius: 3px;"
                                               "line-height: 41px;"
                                               "color: #fff;");
               }
               else if(get[i]=='m')
               {//参数错误情况
                   ui->pushButton->setText("参数错误！");
                   ui->pushButton->setStyleSheet(" background: #ff0000;"
                                               "border: 0;"
                                               "border-radius: 3px;"
                                               "line-height: 41px;"
                                               "color: #fff;");
                   break;
               }
           }
       }
       else
       {//得不到信息
         ui->pushButton->setText("无法连接服务器！5s后自动重试...");
         ui->pushButton->setStyleSheet(" background: #ff0000;"
                                           "border: 0;"
                                           "border-radius: 3px;"
                                           "line-height: 41px;"
                                           "color: #fff;");
         RetryTimer=new QTimer(this);  //this说明是当前类对象的定时器
         connect(RetryTimer,SIGNAL(timeout()),this,SLOT(PostTryTimeSlot()));  //把信号与槽进行连接
         RetryTimer->setSingleShot(true);//单次触发
         RetryTimer->start(5000);
       }
      reply->deleteLater();//回收
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
