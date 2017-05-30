#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QMessageBox"
#include "QTextCodec"
#include "QTimer"
unsigned int usedtime;
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
           ui->LOOUT->setEnabled(false);
           //下面还没做
        }
        else
        {//在线，将信息读取到结构体中
            ui->InfoWidget->setEnabled(true);
            ui->InputWidget->setEnabled(false);
            ui->InfoWidget->setVisible(true);
           ui->InputWidget->setVisible(false);
           ui->LOGIN->setEnabled(false);
           ui->LOOUT->setEnabled(true);
//            getinfo=(struct feedback_info *)new info;
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
                    {
                       //strcpy(getinfo->name,temp);
                       ui->NAME->setText(QString::fromStdString(temp));
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
                       usedtime=atoi(temp);
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
    delete ui;
}
