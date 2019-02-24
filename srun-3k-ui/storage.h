#ifndef STORAGE_H
#define STORAGE_H

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>


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
    QByteArray fileChecksum(const QString &fileName);
    bool checkUpdateProgramisExist(const QString &fileName);
    static size_t saveDataTodisk(void *pBuffer, size_t nSize, size_t nMemByte, FILE *fp);
};



#endif // STORAGE_H
