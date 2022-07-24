#include "mythread.h"
#include "dbfunctions.h"
#include <QSplashScreen>
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include <QTextStream>

myThread::myThread()
{

}

void myThread::run()
{
  qDebug()<<this->currentThread()<<" has started.."; 

  if(this->flag == 2)
  {
      this->connection();
      this->collectingIps();
      qDebug()<<"done ";
  }
  else if(this->flag == 1)
  {
    this->ipAddress = this->ipFound();
    qDebug()<<"done "<<this->ipAddress;
    this->getFoundIp();
  }
}

void myThread::connection()
{
 QSqlDatabase mdbs = QSqlDatabase::addDatabase("QSQLITE");
 mdbs.setDatabaseName("storage/DATABASE.db");
 mdbs.open();

}

void myThread::collectingIps()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString myglobalip = nullptr;
    QStringList ipmade;

    qDebug()<<list;
    // finding global ip

    foreach(QHostAddress line, list)
    {
      if(line.isGlobal())
      {
          myglobalip = line.toString();
      }
    }

    //spit the ip address at .

    QStringList myipparts = myglobalip.split('.');

    //check if list has been made after split

    if(myipparts.isEmpty())
    {
        return;
    }

    // first and second part of ip
    QString first = myipparts.first();
    QString second = myipparts[1];

    //getting number at pos 3 and 4

    int third = myipparts[2].toInt() + 1 * 10;
    int fourth = myipparts.last().toInt() + 1 * 10;
    qDebug()<<third<<fourth;
    // check if first and second varable are not empty

    if(first != nullptr && second != nullptr)
    {

        //generate ip address third position up to 100 and last position upto 100

        for(int i = 1; i <= third; i++)
        {
            for(int j = 1; j <= fourth; j++)
            {
                //joining all part here
                QString ip = first+"."+second+"."+QString::number(i)+"."+QString::number(j);
                ipmade.append(ip);
            }
        }
    }

   //check if ipmade is not null

    if(ipmade.isEmpty())
    {
        return;
    }

    //save all made ips into network table of database for later use

    QSqlQuery query;
    int counter = 0;

    for(int i = 0; i < ipmade.size(); i++)
    {
        query.prepare("INSERT INTO network VALUES('"+ipmade[i]+"');");
        if(query.exec())
        {
            counter++;
        }
    }

    //check if all ips have been saved

    if(counter == ipmade.size())
    {
        qDebug()<<"thread is done making ip addresses ";
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

void myThread::flagging(int x)
{
    this->flag = x;
}

QString myThread::ipFound()
{

    QTcpSocket *sock = new QTcpSocket(0);
    qDebug()<<"here 1"<<ips.size();
    int x = this->ips.size();
    for(int i = 0; i < x; i++)
    {
        qDebug()<<"here 2"<<this->ips.size()<<i;
        sock->connectToHost(this->ips[i],8888);
        if(sock->waitForConnected(1000))
        {
            sock->close();
            return this->ips[i];
        }
        else
        {
            qDebug()<<this->ips[i]<< "Checked";
            if(i == this->ips.size())
            {
                return "127.0.0.1";
            }
            qDebug()<<"here 3"<<this->ips.size()<<i;
            continue;
        }
    }
    sock->deleteLater();
    return "127.0.0.1";

}

QString myThread::getFoundIp()
{
   QDir dir;
   QString file = dir.absoluteFilePath("storage/ipAdress.txt");

   QFile my(file);
   my.open(QIODevice::WriteOnly|QFile::Text);
   if(my.isOpen())
   {
       QTextStream out(&my);
       out<<this->ipAddress;
       my.flush();
       my.close();
   }
}

void myThread::collectall(QStringList list)
{
  this->ips = list;
}
