#ifndef STORAGE_H
#define STORAGE_H

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>


class storage
{
public:
    storage(){}
    ~storage(){}
    bool readServerConfigFromFile(QStringList &list);
    bool readuserConfigFromFile(QStringList &list);
    QByteArray usernameEncrypt(QString username);
    QByteArray passwordEncrypy(QString password);
    bool saveUserConfigToFile(QStringList list);
    bool saveserverConfigToFile(QStringList list);
};



#endif // STORAGE_H
