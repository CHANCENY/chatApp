#include "mythread.h"
#include "dbfunctions.h"
#include <QSplashScreen>


myThread::myThread()
{

}

void myThread::run()
{
  qDebug()<<this->currentThread()<<" has started.."; 
  this->connection();
  this->collectingIps();
  qDebug()<<"done ";

}

void myThread::connection()
{
 QSqlDatabase mdbs = QSqlDatabase::addDatabase("QSQLITE");
 mdbs.setDatabaseName("DATABASE.db");
 mdbs.open();

}

void myThread::collectingIps()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    QString ipAddress = nullptr;

    QStringList ipMadeUp;

    foreach(QHostAddress line, list)
    {
      if(line.isGlobal())
      {
          ipAddress = line.toString();
      }
    }

    int flag = 0;

    if(ipAddress != nullptr)
    {
        DBFunctions ips;
        QStringList dbips = ips.getIpAddress();

        if(dbips.isEmpty())
        {
            flag = -2;
        }

        foreach(QString ipline, dbips)
        {
            if(ipAddress == ipline)
            {
                flag = 0;
                qDebug()<<"All set";
                return;
            }
            flag = -1;
        }

        if(flag < 0)
        {
          QString first = ipAddress.split('.').first();
          QString second = ipAddress.split('.')[1];

          for(int i = 0; i <= 10; i++)
          {
              for(int j = 0; j <=10; j++)
              {
                  QString madeip = first+"."+second+"."+QString::number(i)+"."+QString::number(j);
                  ipMadeUp.append(madeip);

                  if(j == 10 && i == 10)
                  {
                      flag = 2;
                  }
              }
          }
        }
    }
    int counter = 0;
    if(flag == 2)
    {
        QSqlQuery query;
        for(int i = 0; i < ipMadeUp.size(); i++)
        {
             query.prepare("INSERT INTO network values('"+ipMadeUp[i]+"');");
             if(query.exec())
             {
                counter++;
             }
        }
    }


    if(counter == ipMadeUp.size())
    {
         qDebug()<<"done .... threading";
        /* QSqlDatabase d = QSqlDatabase::addDatabase("QSQLITE");
         d.setDatabaseName("DATABASE.db");
         d.open();
         d.close();
         d = QSqlDatabase();
         QSqlDatabase::removeDatabase("DATABASE.db");*/
         return;
    }


}

bool myThread::settingNetworkCheck()
{
  QSqlQuery query;
  query.prepare("SELECT * FROM network;");
  if(query.exec())
  {
      while(query.next())
      {
          if(query.value(0).toString() != nullptr)
          {
              return true;
          }
      }
  }
  return false;
}
