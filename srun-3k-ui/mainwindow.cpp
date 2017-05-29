#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "QMessageBox"
#include "QTextCodec"

int state=0;//定义状态，默认不在线
struct feedback_info *getinfo;//储存信息结构体指针

typedef struct feedback_info
{
    char name[20];
    double data;
    long time;
    char ip[20];
}info;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*自动获取状态*/
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this,SLOT(GET_INFO_Finished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://172.16.154.130/cgi-bin/rad_user_info")));
    /*判断状态*/
    if(state==0)
    {

    }
    if(state==1)
    {

    }
}

void MainWindow::GET_INFO_Finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
     {//得到信息
        QTextCodec *codec =QTextCodec::codecForName("GB2312");
        QString all = codec->toUnicode(reply->readAll());
        std::string get=all.toStdString();
        if(get[0]=='n');//不在线
        else
        {//在线，将信息读取到结构体中
            state=1;
            getinfo=(struct feedback_info *)new info;
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
                       strcpy(getinfo->name,temp);
                   if(flag==7)
                   {
                       getinfo->data=atof(temp)/1073741824;
                   }
                   if(flag==8)
                   {
                       getinfo->time=atoi(temp);
                   }
                   if(flag==9)
                   {
                       strcpy(getinfo->ip,temp);
                       break;
                   }
                   k++;
                   continue;
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


MainWindow::~MainWindow()
{
    delete ui;
}
