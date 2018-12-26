#include "storage.h"


bool storage::readServerConfigFromFile(QStringList &list)
{
    QString serverconfig=QCoreApplication::applicationDirPath()+"/server.json";
    QFile open(serverconfig);
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
                list<<(QString)obj.value("login_server").toString();

           }
         if(obj.contains("service_server"))
            {
              list<<(QString)obj.value("service_server").toString();

           }
         if(obj.contains("mac"))
            {
             list<<(QString)obj.value("mac").toString();

           }
         if(obj.contains("login_port"))
            {
              list<<(QString)obj.value("login_port").toString();

           }
         if(obj.contains("service_port"))
            {
              list<<(QString)obj.value("service_port").toString();

           }
         if(obj.contains("acid"))
            {
              list<<(QString)obj.value("acid").toString();

           }
         if(obj.contains("type"))
            {
              list<<(QString)obj.value("type").toString();

           }
         if(obj.contains("drop"))
            {
              list<<(QString)obj.value("drop").toString();

           }
         if(obj.contains("pop"))
            {
              list<<(QString)obj.value("pop").toString();
           }
           return true;
        }
    }
    else
    {
        return false;
    }
}


bool storage::readuserConfigFromFile(QStringList &list)
{
    QString userconfig=QCoreApplication::applicationDirPath()+"/config.json";
    QFile u(userconfig);
    if(u.open(QIODevice::ReadOnly))
    {

        QByteArray OPEN_USERINFO=u.readAll();
        u.close();
        QJsonDocument USERINFO=QJsonDocument::fromJson(OPEN_USERINFO);
        if(USERINFO.isObject())
        {
              QJsonObject obj=USERINFO.object();
              if(obj.contains("username"))
                 list<<obj.value("username").toString();
              if(obj.contains("password"))
                 list<<obj.value("username").toString();
              if(obj.contains("auto_start"))
                 list<<obj.value("auto_start").toString();
              if(obj.contains("auto_login"))
                 list<<obj.value("auto_login").toString();
              return true;
         }
    }
    return false;
}

bool storage::saveUserConfigToFile(QStringList list)
{
    QJsonObject info;
    info.insert("username",list.at(0));
    info.insert("password",list.at(1));
    info.insert("auto_login",list.at(2));
    info.insert("auto_start",list.at(3));
    QJsonDocument SAVE_INFO;
    SAVE_INFO.setObject(info);
    QString userconfig=QCoreApplication::applicationDirPath()+"/config.json";
    QFile save(userconfig);
    if(!save.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    else
    {
        save.write(SAVE_INFO.toJson());
    }
    save.close();
    return true;
}

bool storage::saveserverConfigToFile(QStringList list)
{
       QJsonObject config;
       config.insert("login_server",list.at(0));
       config.insert("service_server",list.at(1));
       config.insert("mac",list.at(2));
       config.insert("login_port",list.at(3));
       config.insert("service_port",list.at(4));
       config.insert("acid",list.at(5));
       config.insert("type",list.at(6));
       config.insert("drop",list.at(7));
       config.insert("pop",list.at(8));
       QJsonDocument SAVE_CONFIG;
       SAVE_CONFIG.setObject(config);
       QString serverconfig=QCoreApplication::applicationDirPath()+"/server.json";
       QFile save(serverconfig);
       if(!save.open(QIODevice::WriteOnly | QIODevice::Truncate))
       {
            return false;
       }
       else
       {
          save.write(SAVE_CONFIG.toJson());
       }
       save.close();
       return true;
}

QByteArray storage::usernameEncrypt(QString username)
{

    QByteArray NAME_ENCRYPT="";
    for (int i=0;i<username.size();i++)
    {//用户名加密
        char c=username.at(i).toLatin1();

        NAME_ENCRYPT.append(QString(c+4));
    }
    return NAME_ENCRYPT;
}

QByteArray storage::passwordEncrypy(QString passwd)
{

    QByteArray PASSWD_ENCRYPT="";
    char key[]= "1234567890";
    for (int i = 0; i<passwd.size(); ++i)
    {//这是密码加密函数
        char password=passwd.at(i).toLatin1();
        int ki = (password) ^ key[strlen(key) - i%strlen(key) - 1];
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
    return PASSWD_ENCRYPT;
}
