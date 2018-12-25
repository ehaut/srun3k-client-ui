#include "version.h"

int checkVersion(QStringList &list,network *n)
{
   QString get=n->httpGet(checkUpdateURL);
   list=n->parseUpdateMessage(get);
   if(list.size()!=0||!list.isEmpty())
   {
        if(strcmp(list.at(0).toStdString().c_str(),version)!=0)
            return 1;
        else
        {
            return 0;
        }
   }
   else
   {
        return -1;
   }
}
