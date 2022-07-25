#include "mysocktes.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QList>
#include "mythread.h"


QString ipfound = nullptr;
static int lock;

mySocktes::mySocktes(QObject *parent) : QTcpSocket(parent)
{
   socket = new QTcpSocket(this);
   connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

}

bool mySocktes::sendingMessages(QByteArray mData)
{
    //attempt to collect ip
    if(ipfound == nullptr)
    {
        DBFunctions i;
        ipfound = i.fetchAvailableIpAddressServer();
    }

    // checking if ipfound is null if so then we run thread of lookup ips
     myThread *thread = new myThread;
    if(ipfound == nullptr && lock == 0)
    {
       lock = 2;
       DBFunctions i;
       thread->flagging(1);
       thread->collectall(i.getIpAddress());
       thread->start();
    }

    // thread will wirte available ip in p=ipaddress.txt file it where this fetching function will look

    if(ipfound == nullptr)
    {
        DBFunctions i;
        ipfound = i.fetchAvailableIpAddressServer();
        thread->deleteLater();
        return false;
    }

    // check if ipfound is not null to procceed and send outgoing data packets

    if(ipfound != nullptr)
    {
        // checking if socket is open so that should not attempt to request for connection
       if(socket->isOpen())
       {
           socket->write(mData);

           // when all sudden server went off it will not waitfor bytewritten
           if(socket->state() == 3)
           {
               qDebug()<<"waitbytewritten";
               socket->waitForBytesWritten(3000);
                return true;
           }
           else
           {
               qDebug()<<"disconnecting";
               socket->close();
               return false;
           }
        thread->deleteLater();
       }

       socket->connectToHost(ipfound,9846);
       if(socket->waitForConnected(1000))
       {
           socket->write(mData);

           // when all sudden server went off it will not waitfor bytewritten
           if(socket->state() == 3)
           {
               qDebug()<<"waitbytewritten";
               socket->waitForBytesWritten(3000);
                return true;
           }
           else
           {
               qDebug()<<"disconnecting ";
               socket->close();
               return false;
           }

       }
       else
       {
           qDebug()<<"clear found ip";
           ipfound.clear();
           this->clearip();
           lock = 0;
           return false;
       }
    }
 thread->deleteLater();
 return false;
}

void mySocktes::writeReciever(QByteArray data)
{
    QDir dir;
    QString filename = dir.absoluteFilePath("DATA");

     QString messages = nullptr;
    QFile myfile(filename);
    myfile.open(QIODevice::WriteOnly|QFile::Text);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
        in<<data;
        myfile.flush();
        myfile.close();
    }
}

QByteArray mySocktes::getMessage()
{
  if(socket->state() == 0)
   {
      return ":/ic/icons/cloud-computing.png";
   }
   if(socket->state() == 1)
   {
      return ":/ic/icons/computing-cloud.png";
   }
   if(socket->state() == 2)
   {
      return ":/ic/icons/computing-cloud.png";
   }
   if(socket->state() == 3)
   {
      return ":/ic/icons/computing-cloud.png";
   }
   if(socket->state() == 4)
   {
      return ":/ic/icons/link.png";
   }
   if(socket->state() == 6)
   {
      return ":/ic/icons/link.png";
   }
   if(socket->state() == 6)
   {
      return ":/ic/icons/link.png";
   }
   return socket->errorString().toLocal8Bit();

}

void mySocktes::clearip()
{
    QDir dir;
    QString file = dir.absoluteFilePath("storage/ipAdress.txt");

    QFile my(file);
    my.open(QIODevice::WriteOnly|QFile::Text);
    if(my.isOpen())
    {
        QTextStream out(&my);
        out<<"";
        my.flush();
        my.close();
    }
}

void mySocktes::readyRead()
{
    QByteArray data = socket->readAll();    
    writeReciever(data);
    qDebug()<<"incoming message...."+data;
    socket->close();
}


