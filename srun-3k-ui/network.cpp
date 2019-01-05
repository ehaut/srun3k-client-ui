#include "network.h"
//#include "QDebug"

size_t network::writefunc(void *ptr,size_t size,size_t nmemb,std::string &str)
{

    size_t total = size * nmemb;
    if (total)
    {
        str.append((char*)ptr,total);
    }
    return total;
}

QString network::httpGet(const char * url)
{

    std::string reback;
    curl = curl_easy_init(); //curl初始化
    if(!(curl))
    {//如果非正常初始化
        exit(-1);
    }

    curl_easy_setopt(curl,CURLOPT_URL,url);//设置GET地址
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,false);//设定为不验证证书和HOST
    curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,false);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, network::writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &reback);
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
    res = curl_easy_perform(curl);//启动libcurl
    if(res!=CURLE_OK)
    {
        isTimeOut=true;
        return "";
    }
    else
    {
        QString result=QString::fromStdString(reback);
        if(result.length()!=0&&!result.isEmpty())
        {
            isTimeOut=false;
            return result;
        }
        else
        {
            isTimeOut=true;
            return "";
        }
    }
    curl_easy_cleanup(curl);
    curl=nullptr;
}

QString network::httpPost(const char * url,const char * post)
{
    std::string reback;
    curl = curl_easy_init(); //curl初始化
    if(!(curl))
    {//如果非正常初始化
        exit(-1);
    }
    curl_easy_setopt(curl,CURLOPT_URL,url);//设置POST地址
    curl_easy_setopt(curl, CURLOPT_POST,1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,10L);//超时设置成
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, network::writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &reback);
    res = curl_easy_perform(curl);//启动libcurl
    if(res!=CURLE_OK)
    {
        isTimeOut=true;
        return "";
    }
    else
    {
        QString result=QString::fromStdString(reback);
        if(result.length()!=0&&!result.isEmpty())
        {
            isTimeOut=false;
            return result;
        }
        else
        {
            isTimeOut=true;
            return "";
        }
    }
    curl_easy_cleanup(curl);
    curl=nullptr;
}


QString network::urlEncode(const QString input)
{
    QString reback;
    curl = curl_easy_init(); //curl初始化
    if(!(curl))
    {//如果非正常初始化
        exit(-1);
    }
    char *output=curl_easy_escape(curl,input.toStdString().c_str(),0);
    if(output)
    {
        reback.sprintf("%s",output);
        curl_free(output);
    }
    curl_easy_cleanup(curl);
    curl=nullptr;
    return reback;
}

QStringList network::parseUserInfo(const QString userinfo,int &usedTime,bool &isOnline)
{

    /**
        list[0] username
        list[1] ip
        list[2] data
        list[3] time
    */
    QStringList list;
    if(userinfo.indexOf("not_online")==-1)
    {
        isOnline=true;
        QStringList getinfo=userinfo.split(",");
        int loginTime=getinfo.at(1).toInt();
        int serverTime=getinfo.at(2).toInt();
        usedTime=getinfo.at(7).toInt()+serverTime-loginTime;
        double data=getinfo.at(6).toDouble()/1073741824;
        list<<getinfo.at(0);
        list<<getinfo.at(8);
        list<<QString::number(data,'g',6);
        list<<QString::number(usedTime);
    }
    else
         isOnline=false;
    return list;
}

QString network::parseServerReback(QString getReback,int &status)
{
//    "login_ok@登陆中...登陆成功",
//    "logout_ok@注销成功",
//    "missing_required_parameters_error@登陆中...缺少参数！",
//    "login_error#E2553: Password is error.@登陆中...密码错误！",
//    "login_error#E2531: User not found.@登陆中...用户名未找到！",
//    "login_error#INFO failed, BAS respond timeout.@ACID错误",
//    "You are not online.@您不在线！",
    status=-1;
    if(getReback.indexOf("login_ok")!=-1)
    {
       status=0;
       getReback="登陆中...登陆成功";
    }
    else if(getReback.indexOf("logout_ok")!=-1)
    {
        status=1;
        getReback="注销中...注销成功";
    }
    else if(getReback.indexOf("missing_required_parameters_error")!=-1)
    {
        getReback="登陆中...缺少参数！";
    }
    else if(getReback.indexOf("login_error#E2553: Password is error.")!=-1)
    {
        getReback="登陆中...密码错误！";
    }
    else if(getReback.indexOf("login_error#E2531: User not found.")!=-1)
    {
        getReback="登陆中...用户名未找到！";
    }
    else if(getReback.indexOf("login_error#INFO failed, BAS respond timeout.")!=-1)
    {
        status=5;
        getReback="登陆中...ACID错误！";
    }
    else if(getReback.indexOf("You are not online.")!=-1)
    {
        status=6;
        getReback="注销中...您不在线！";
    }
    //    "login_error#E2901: (Third party 1)Status_Err@登陆中...你的账户状态错误可能是您欠费了！",
    //    "login_error#E2901: (Third party 1)User Locked@登陆中...你的账户被锁定可能是您欠费了！",
    //    "ip_already_online_error@登陆中...您的IP已经在线了"
    else if(getReback.indexOf("login_error#E2901: (Third party 1)Status_Err")!=-1)
    {
        getReback="登陆中...你的账户状态错误，可能是您欠费了！";
    }
    else if(getReback.indexOf("login_error#E2901: (Third party 1)User Locked")!=-1)
    {
        getReback="登陆中...登陆中...你的账户被锁定，可能是您欠费了！";
    }
    else if(getReback.indexOf("ip_already_online_error")!=-1)
    {
        getReback="登陆中...您的IP已经在线了！";
    }
    return getReback;
}

QStringList network::parseServerMessage(QString getReback)
{
    /**
      list[0] title
      list[1] content
    */
    QStringList list;
    QByteArray byte=getReback.toUtf8();
    QJsonDocument info = QJsonDocument::fromJson(byte);
    QJsonObject obj=info.object();
    if(obj.contains("Data"))
    {
        QJsonValue value=obj.value("Data");
        QJsonArray message=value.toArray();
        QJsonValue info = message.at(0);
        list<<info.toObject().value("Title").toString();
        list<<info.toObject().value("Content").toString();
    }
    return list;
}

QStringList network::parseUpdateMessage(QString getReback)
{
    QStringList list;
    /**
      list[0] version
      list[1] date
      list[2] author
      list[3] sha1
      list[4] url
    */
    QByteArray byte=getReback.toUtf8();
    QJsonDocument info = QJsonDocument::fromJson(byte);
    QJsonObject obj=info.object();
    list<<obj.value("version").toString();
    list<<obj.value("date").toString();
    list<<obj.value("author").toString();
    list<<obj.value("sha1").toString();
    list<<obj.value("url").toString();
    return list;
}
