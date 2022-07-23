#include "registration.h"
#include "dbfunctions.h"
#include "chatroom.h"
#include "loginpage.h"
#include "steptwo.h"
#include <QStringList>
#include "mythread.h"
#include "networkdilog.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myThread mythread;
    mythread.connection();
    if(mythread.settingNetworkCheck() == false)
    {
        DBFunctions owner;
        owner.connectionSetUp();
        owner.tablesMaker();
        mythread.connection();
        mythread.start();

        networkDilog dia(nullptr, &mythread);
        dia.show();
        return a.exec();
    }

    DBFunctions owner;
    owner.connectionSetUp();

    if(owner.findingUserExist())
    {

       QStringList userdetails = owner.collectOwner();

       //check if login enabled
       if(owner.checkSecuritySetting())
       {
           loginpage login(nullptr,&userdetails);
           login.show();
           return a.exec();
       }
       else
       {
           // check step two verification

           if(owner.checkSecuritySettingTwo())
           {
               QString phone = userdetails.at(1);
               StepTwo two(nullptr,&phone);
               two.show();
               return a.exec();
           }
           else
           {

               ChatRoom chat(nullptr,userdetails.last(),userdetails.at(1),userdetails.at(0));
               chat.show();
               return a.exec();
           }

       }
    }
    else
    {
        Registration w;
        w.show();
        return a.exec();
    }

    return a.exec();
}
