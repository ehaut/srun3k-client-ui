#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QMessageBox"
#include "QFile"
#include "QSettings"
#include <curl/curl.h>
#include <string>

const char POSTURL[]= "http://172.16.154.130:69/cgi-bin/srun_portal";
int file_state=0;

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

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    QMessageBox::critical(NULL,"错误!","无法分配内存");
    QCoreApplication::exit();
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

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
              ui->SAVE_LOGIN->setCheckState(Qt::Checked);
               if(obj.contains("auto_start"))
                  {
                       bool auto_start=obj.value("auto_start").toBool();
                       if(auto_start)
                            ui->AUTO_START->setCheckState(Qt::Checked);
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

    QString NAME_INPUT = ui->INPUT_NAME->text().trimmed();
    QString PASSWD_INPUT = ui->INPUT_PASSWD->text();

    if(NAME_INPUT.isEmpty()||PASSWD_INPUT.isEmpty()||(NAME_INPUT.isEmpty()&&PASSWD_INPUT.isEmpty()))
     {
        QMessageBox::critical(this, tr("错误!"),tr("请输入您的用户名和密码登陆"));
    }
    if(!(NAME_INPUT.isEmpty())&&!(PASSWD_INPUT.isEmpty()))
     {
        QByteArray temp1 = NAME_INPUT.toLatin1();
           char *name = temp1.data();
         QByteArray temp2 = PASSWD_INPUT.toLatin1();
            char *password = temp2.data();
            char post[200]="&action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&ac_id=1&username=%7BSRUN3%7D%0D%0A";
            char *ptr=post;
            while(*ptr!='\0')ptr++;//到达POST参数末尾
            char temp[50]="\0";//这是用来临时存储加密后用户名和密码
            char *t=temp;
                for (;*name!='\0';name++,t++)
                {//用户名加密
                    *t=*name+4;
                }
                *t='\0';
                CURL *curl1 = curl_easy_init();//使用libcurl完成urlencode
                    CURL *curl2 = curl_easy_init();//当时测试一个完成用户名和密码urlencode会造成程序崩溃，所以此处用了两个分别完成
                    if(!curl1&&!curl2)
                    {//如果非正常初始化
                        QMessageBox::critical(NULL,"错误!","LIBCURL初始化错误");
                        QCoreApplication::exit();
                    }
                    char *name_urlencode = curl_easy_escape(curl1,temp,0);//对名字urlencode
                    while(*name_urlencode!='\0')
                    {//复制urlencode后的名字到POST参数字符数组
                        *ptr=*name_urlencode;
                        ptr++;
                        name_urlencode++;
                    }
                    memset(temp,0,sizeof(temp));//将temp字符数组重置
                    char key[] = "1234567890";//加密key
                        int i;
                        for (i = 0; *(password+i)!='\0'; ++i)
                        {//这是密码加密函数
                            int ki = *(password+i) ^ key[strlen(key) - i%strlen(key) - 1];
                            char _l[4] = { (char)((ki & 0x0f) + 0x36) };
                            char _h[4] = { (char)((ki >> 4 & 0x0f) + 0x63) };
                            if (i % 2 == 0)
                            {
                                strcat(_l, _h);
                                strcat(temp, _l);
                            }
                            else
                            {
                                strcat(_h, _l);
                                strcat(temp, _h);
                            }
                        }
                        for(char *password_Post="&password=";*password_Post!='\0';password_Post++,ptr++)
                        {
                            *ptr=*password_Post;
                        }
                        char *password_urlencode = curl_easy_escape(curl2,temp,0);
                        while(*password_urlencode !='\0')
                        {
                            *ptr=*password_urlencode ;
                            ptr++;
                            password_urlencode++;
                        }
                        *ptr='\0';//设置字符数组结束位
                        curl_easy_cleanup(curl1);//清理
                        curl_easy_cleanup(curl2);//清理
                        struct MemoryStruct chunk;
                        chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
                        chunk.size = 0;    /* no data at this point */
                        curl_global_init(CURL_GLOBAL_ALL);
                        /* init the curl session */
                       CURL * curl = curl_easy_init();
                         if(!curl)
                            {
                             QMessageBox::critical(NULL,"错误!","LIBCURL初始化错误");
                             QCoreApplication::exit();
                         }
                        /* specify URL to get */
                        curl_easy_setopt(curl, CURLOPT_URL, POSTURL);

                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数

                        /* send all data to this function  */
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

                        curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
                        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                        /* we pass our 'chunk' struct to the callback function */
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

                        /* some servers don't like requests that are made without a user-agent
                           field, so we provide one */

                        /* get it! */
                        CURLcode res = curl_easy_perform(curl);
                         char *feed=chunk.memory;
                        /* check for errors */
                        if(res != CURLE_OK) {
                          QMessageBox::critical(NULL,"错误!","无法连接服务器!");
                        }
                        else
                        {
                            for(;*chunk.memory!='\0';chunk.memory++)
                            {
                                if(*chunk.memory=='n'&&*(chunk.memory+2)=='o')
                                        {//登陆成功情况
                                            if(file_state==0&&ui->SAVE_LOGIN->isChecked())
                                            {
                                                    QJsonObject info;
                                                     info.insert("username",NAME_INPUT);
                                                     info.insert("password",PASSWD_INPUT);
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
                                                         QMessageBox::information(this, tr(":) 登陆成功且配置文件保存成功!"),tr("您已登陆成功!\n配置文件已经保存本程序目录下，\n下次打开本程序将会自动载入。\n为了保证信息完整性，请勿更改!!!"));
                                                     }
                                                     save.close();
                                             }
                                             else
                                             {
                                                 QMessageBox::information(this, tr(":) 登陆成功!"),tr("您已登陆成功!"));
                                             }
                                            break;
                                        }
                                else if(*chunk.memory=='P'&&*(chunk.memory+1)=='a')
                                        {//密码错误情况
                                            QMessageBox::critical(this,tr(":( 密码错误!"),tr("密码错误，请检查输入后重试!"));
                                            break;
                                        }
                                else if(*chunk.memory=='U'&&*(chunk.memory+1)=='s')
                                        {//用户名错误情况
                                            QMessageBox::critical(this,tr(":( 用户名错误!"),tr("用户名错误，请检查输入后重试!"));
                                            break;
                                        }
                                else if(*chunk.memory=='I'&&*(chunk.memory+1)=='N')
                                        {//ACID错误情况
                                            QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
                                            break;
                                        }
                                else if(*chunk.memory=='m')
                                        {//参数错误情况
                                           QMessageBox::critical(this,tr(":( 参数错误!"),tr("参数错误，请重新输入重试!"));
                                            break;
                                        }
                            }
                        }

                        /* cleanup curl stuff */
                        curl_easy_cleanup(curl);

                        free(feed);

                        /* we're done with libcurl, so clean it up */
                        curl_global_cleanup();
    }
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
        QByteArray temp = NAME_INPUT.toLatin1();
           char *name = temp.data();
           char post[100]="&mac=&type=2&action=logout&ac_id=1&username=";
           strcat(post,name);
           struct MemoryStruct chunk;
           chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
           chunk.size = 0;    /* no data at this point */
           curl_global_init(CURL_GLOBAL_ALL);
           /* init the curl session */
          CURL * curl = curl_easy_init();
            if(!curl)
               {
                QMessageBox::critical(NULL,"错误!","LIBCURL初始化错误");
                QCoreApplication::exit();
            }
           /* specify URL to get */
           curl_easy_setopt(curl, CURLOPT_URL, POSTURL);

           curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数

           curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
           /* send all data to this function  */
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

           /* we pass our 'chunk' struct to the callback function */
           curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

           /* some servers don't like requests that are made without a user-agent
              field, so we provide one */

           /* get it! */
           CURLcode res = curl_easy_perform(curl);
           char *ptr=chunk.memory;
           /* check for errors */
           if(res != CURLE_OK) {
             QMessageBox::critical(NULL,"错误!","无法连接服务器!");
           }
           else
           {
               for(;*chunk.memory!='\0';chunk.memory++)
               {
                   if(*chunk.memory=='t'&&*(chunk.memory+2)=='o')
                           {//注销成功情况
                              QMessageBox::information(this, tr(":) 注销成功!"),tr("您已注销成功!"));
                              break;
                           }
                   else if(*chunk.memory=='Y'&&*(chunk.memory+1)=='o')
                           {//不在线情况
                               QMessageBox::critical(this,tr(":( 注销失败!"),tr("您不在线，无法完成注销!"));
                               break;
                           }
                   else if(*chunk.memory=='I'&&*(chunk.memory+1)=='N')
                           {//ACID错误情况
                               QMessageBox::critical(this,tr(":( ACID错误!"),tr("ACID错误，请更改ACID后重试!"));
                               break;
                           }
                   else if(*chunk.memory=='m')
                           {//参数错误情况
                              QMessageBox::critical(this,tr(":( 参数错误!"),tr("参数错误，请重新输入重试!"));
                               break;
                           }
               }
           }

           /* cleanup curl stuff */
           curl_easy_cleanup(curl);

           free(ptr);

           /* we're done with libcurl, so clean it up */
           curl_global_cleanup();
    }
}

void MainWindow::on_INFO_clicked()
{
    CURL *curl_handle;
      CURLcode res;

      struct MemoryStruct chunk;

      chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
      chunk.size = 0;    /* no data at this point */

      curl_global_init(CURL_GLOBAL_ALL);

      /* init the curl session */
      curl_handle = curl_easy_init();
       if(!curl_handle)
          {
           QMessageBox::critical(NULL,"错误!","LIBCURL初始化错误");
           QCoreApplication::exit();
       }
      /* specify URL to get */
      curl_easy_setopt(curl_handle, CURLOPT_URL, "http://172.16.154.130/cgi-bin/rad_user_info");

      /* send all data to this function  */
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

      curl_easy_setopt(curl_handle,CURLOPT_TIMEOUT,1L);//超时设置成1s
       curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
      /* we pass our 'chunk' struct to the callback function */
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

      /* some servers don't like requests that are made without a user-agent
         field, so we provide one */

      /* get it! */
      res = curl_easy_perform(curl_handle);
      char *back=chunk.memory;
      /* check for errors */
      if(res != CURLE_OK) {
        QMessageBox::critical(NULL,"错误!","无法连接服务器!");
      }
      else
      {
          if(*chunk.memory=='n')
                QMessageBox::information(this, tr(":) 您的信息!"),tr("您不在线!"));
          else
          {
          char *j=chunk.memory;
          info i;
          char temp[20]={0}; //定义一个临时变量
          for(int flag=0;chunk.memory!='\0';chunk.memory++)//开始循环直到数据末尾
          {
               memset(temp,0,sizeof(temp));//将temp字符数组重置
              if(*chunk.memory==',')//如果当前位为,分隔符
              {
                  flag++;//,分隔符计数
                  char *t=temp;//定义一个临时指针指向t
                  for(;*j!=*chunk.memory;j++,t++)
                      *t=*j;
                  *t='\0';
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
                  j++;
                  continue;
              }
          }
          double data_used=atof(i.data)/1073741824;
          int time_h=atoi(i.time)/3600;
          int time_m=atoi(i.time)/60%60;
          int time_s=atoi(i.time)%60;
         QString put=+"您已在线，下面是您的登录信息:\n您的登入用户名是:"+QString(i.name)+"\n"+"您这个月所用流量:"+ QString::number(data_used,'g',6)+"GB\n"+"您这个月所用时间:"+\
                 QString::number(time_h,10)+"时"+ QString::number(time_m,10)+"分"+ QString::number(time_s,10)+"秒\n"+"您 的 登 入  IP:"+QString(i.ip);
          QMessageBox::information(this, tr(":) 您的信息!"),put);
          }
      }
      /* cleanup curl stuff */
      curl_easy_cleanup(curl_handle);

      free(back);
      /* we're done with libcurl, so clean it up */
      curl_global_cleanup();
}

void MainWindow::on_GET_MESSAGE_clicked()
{
    CURL *curl_handle;
      CURLcode res;

      struct MemoryStruct chunk;

      chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
      chunk.size = 0;    /* no data at this point */

      curl_global_init(CURL_GLOBAL_ALL);

      /* init the curl session */
      curl_handle = curl_easy_init();
       if(!curl_handle)
          {
           QMessageBox::critical(NULL,"错误!","LIBCURL初始化错误");
           QCoreApplication::exit();
       }
      /* specify URL to get */
      curl_easy_setopt(curl_handle, CURLOPT_URL, "http://172.16.154.130/get_msg.php");

      /* send all data to this function  */
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

      curl_easy_setopt(curl_handle,CURLOPT_TIMEOUT,1L);//超时设置成1s
     curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
      /* we pass our 'chunk' struct to the callback function */
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

      /* some servers don't like requests that are made without a user-agent
         field, so we provide one */

      /* get it! */
      res = curl_easy_perform(curl_handle);
      /* check for errors */
      if(res != CURLE_OK) {
       QMessageBox::critical(NULL,"错误!","无法连接服务器!");
      }
      else
      {
           QMessageBox::information(this, tr(":) 公告信息"),QString::fromLocal8Bit(chunk.memory));
      }
      curl_easy_cleanup(curl_handle);

      free(chunk.memory);
      /* we're done with libcurl, so clean it up */
      curl_global_cleanup();
}

void MainWindow::on_AUTO_START_clicked()
{
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
