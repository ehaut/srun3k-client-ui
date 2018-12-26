#include "network.h"
#include "QDebug"

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
        return "";
    }
    else
    {
        QString result=QString::fromStdString(reback);
        return result;
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
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post);//设置POST参数
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, network::writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &reback);
    res = curl_easy_perform(curl);//启动libcurl
    if(res!=CURLE_OK)
    {
        return "";
    }
    else
    {
        QString result=QString::fromStdString(reback);
        return result;
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

    if(getReback.isEmpty())
        status=-2;
    else
    {
        QStringList list=serverReback.split(",");
        for(int i=0;i<list.size();i++)
        {
            if(i==0)
                status=1;
            else if(i==1)
                status=0;
            else
            {
                status=-1;
            }
            if(getReback.indexOf(list.at(i).split("@").at(0))!=-1)
            {
                getReback=list.at(i).split("@").at(1);
                break;
            }
        }
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
    if(!obj.isEmpty())
    {
        list<<obj.value("version").toString();
        list<<obj.value("date").toString();
        list<<obj.value("author").toString();
        list<<obj.value("sha1").toString();
        list<<obj.value("url").toString();
    }
    return list;
}
