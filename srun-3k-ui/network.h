#ifndef NETWORK_H
#define NETWORK_H
#include "curl/curl.h"
#include "string.h"
#include "QString"
#include "QStringList"
#include "QJsonObject"
#include "QJsonArray"
#include "QJsonDocument"

static QString serverReback=   "login_ok@登陆成功,"
                        "logout_ok@注销成功,"
                        "missing_required_parameters_error@缺少参数,"
                        "login_error#E2553: Password is error.@密码错误,"
                        "login_error#E2531: User not found.@用户名未找到,"
                        "login_error#INFO failed, BAS respond timeout.@ACID错误,"
                        "login_error#You are not online.@您不在线,"
                        "login_error#E2901: (Third party 1)Status_Err@你的账户状态错误,可能是您欠费了,"
                        "login_error#E2901: (Third party 1)User Locked@你的账户被锁定,可能是您欠费了,"
                        "ip_already_online_error@您的IP已经在线了";
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
};


#endif // NETWORK_H
