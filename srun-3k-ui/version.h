#ifndef VERSION_H
#define VERSION_H
#include "network.h"

static char * version="V2.0.1";
static char * checkUpdateURL="https://pc.ehaut.cn/download/checkupdate.json";
int checkVersion(QStringList &list,network *n);


#endif // VERSION_H
