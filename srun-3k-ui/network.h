#ifndef NETWORK_H
#define NETWORK_H
#include "curl/curl.h"
#include "string.h"
#include "QString"
#include "QStringList"
#include "QJsonObject"
#include "QJsonArray"
#include "QJsonDocument"


class network
{
public:
    network()
    {
         curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    ~network()
    {
         curl_global_cleanup();
    }
private:
    CURL *curl=nullptr;
    CURLcode res;
    static size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string &str);
public:
    QString httpGet(const char * url);
    QString urlEncode(const QString input);
    QString httpPost(const char *url, const char *post);
    QString parseServerReback(QString getReback, int &status);
    QStringList parseUserInfo(const QString userinfo, int &usedTime, bool &isOnline);
    QStringList parseServerMessage(QString getReback);
    QStringList parseUpdateMessage(QString getReback);
    bool isTimeOut;
};


#endif // NETWORK_H
