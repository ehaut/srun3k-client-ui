#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <curl/curl.h>
#include <string.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LOGIN_clicked()
{

    QString NAME_INPUT = ui->INPUT_NAME->text();
    QString PASSWD_INPUT = ui->INPUT_PASSWD->text();
    if(NAME_INPUT.isEmpty())//如果用户名是空的
     {
        QMessageBox::critical(this, tr("错误!"),tr("请输入您的用户名登陆"));
    }
    if(PASSWD_INPUT.isEmpty())//如果密码是空的
    {
       QMessageBox::critical(this, tr("错误!"),tr("请输入您的密码登陆"));
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
                        curl_easy_setopt(curl, CURLOPT_URL, "http://172.16.154.130:69/cgi-bin/srun_portal");

                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数

                        /* send all data to this function  */
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

                        curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s

                        /* we pass our 'chunk' struct to the callback function */
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

                        /* some servers don't like requests that are made without a user-agent
                           field, so we provide one */

                        /* get it! */
                        CURLcode res = curl_easy_perform(curl);

                        /* check for errors */
                        if(res != CURLE_OK) {
                          QMessageBox::critical(NULL,"错误!","无法连接服务器!");
                        }
                        else {
                          /*
                           * Now, our chunk.memory points to a memory block that is chunk.size
                           * bytes big and contains the remote file.
                           *
                           * Do something nice with it!
                           */

                          QMessageBox::information(this, tr("接受信息!"),QString(chunk.memory));
                        }

                        /* cleanup curl stuff */
                        curl_easy_cleanup(curl);

                        free(chunk.memory);

                        /* we're done with libcurl, so clean it up */
                        curl_global_cleanup();
    }
}

void MainWindow::on_LOGOUT_clicked()
{
    QString NAME_INPUT = ui->INPUT_NAME->text();
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
           curl_easy_setopt(curl, CURLOPT_URL, "http://172.16.154.130:69/cgi-bin/srun_portal");

           curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数

           curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s

           /* send all data to this function  */
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

           /* we pass our 'chunk' struct to the callback function */
           curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

           /* some servers don't like requests that are made without a user-agent
              field, so we provide one */

           /* get it! */
           CURLcode res = curl_easy_perform(curl);

           /* check for errors */
           if(res != CURLE_OK) {
             QMessageBox::critical(NULL,"错误!","无法连接服务器!");
           }
           else {
             /*
              * Now, our chunk.memory points to a memory block that is chunk.size
              * bytes big and contains the remote file.
              *
              * Do something nice with it!
              */

             QMessageBox::information(this, tr("接受信息!"),QString(chunk.memory));
           }

           /* cleanup curl stuff */
           curl_easy_cleanup(curl);

           free(chunk.memory);

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
      else {

        QMessageBox::information(this, tr("接受信息!"),QString(chunk.memory));
      }

      /* cleanup curl stuff */
      curl_easy_cleanup(curl_handle);

      free(chunk.memory);

      /* we're done with libcurl, so clean it up */
      curl_global_cleanup();
}
