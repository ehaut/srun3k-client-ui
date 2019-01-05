#include "version.h"
//#include "QDebug"


int checkVersion(QStringList &list,network *n)
{
   QString get=n->httpGet(checkUpdateURL);
   QString notOnLineTip="open the authentication page";
   //qDebug()<<get;
   if(get.indexOf(notOnLineTip)==-1)
   {
        if(!n->isTimeOut)
        {
            list=n->parseUpdateMessage(get);
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
   else
   {
        return -1;
   }
}
