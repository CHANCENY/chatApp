#include "mysocktes.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QList>

 QStringList availableips;
 QStringList ips;
 static int lock;

mySocktes::mySocktes(QObject *parent) : QTcpSocket(parent)
{
   socket = new QTcpSocket(this);
   connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

}

bool mySocktes::sendingMessages(QByteArray mData)
{
    DBFunctions ip;   

     if(ips.isEmpty())
     {
         ips = ip.getIpAddress();
     }

    if(ips.isEmpty())
    {
        return false;
    }

    if(lock == 1)
    {
        socket->waitForDisconnected(2000);

    }

    if(availableips.isEmpty())
    {
        lock = 1;
        qDebug()<<"long list ";
        for(int i = 0; i < ips.size(); i++)
        {
            QString ipaddress = ips[i];
            QHostAddress address;
            address.setAddress(ipaddress);

            if(address.isGlobal())
            {
                if(socket->isOpen())
                {
                    qDebug()<<"in socket sending..."+mData;
                    if(socket->waitForConnected(1000))
                    {
                        qDebug()<<"outgoing message...."+mData;
                        socket->write(mData);
                        availableips.append(ipaddress);
                        lock = 0;
                        return true;
                    }
                }

                socket->connectToHost(address,8888);
                qDebug()<<"in socket sending..."+mData;
                if(socket->waitForConnected(1000))
                {
                    qDebug()<<"outgoing message...."+mData;
                    socket->write(mData);
                    availableips.append(ipaddress);
                    lock = 0;
                    return true;
                }
                else
                {

                    if(ipaddress == ips.last())
                    {
                        qDebug()<<"sending failed...";
                        socket->close();
                        //availableips.removeAt(i);
                        lock = 0;
                        return false;
                    }
                   continue;
                }
            }
        }
    }
    else
    {
        lock = 1;
        qDebug()<<"available list";
        for(int i = 0; i < availableips.size(); i++)
        {
            QString ipaddress = availableips[i];
            QHostAddress address;
            address.setAddress(ipaddress);

            if(socket->isOpen())
            {
                qDebug()<<"in socket sending..."+mData;
                if(socket->waitForConnected(1000))
                {
                    qDebug()<<"outgoing message...."+mData;
                    socket->write(mData);
                    availableips.append(ipaddress);
                    lock = 0;
                    return true;
                }
            }

            socket->connectToHost(address,8888);
            qDebug()<<"in socket sending..."+mData;
            if(socket->waitForConnected(1000))
            {
                qDebug()<<"outgoing message...."+mData;
                socket->write(mData);
                lock = 0;
                return true;
            }
            else
            {
                if(ipaddress == availableips.last())
                {
                    qDebug()<<"sending failed...";
                    socket->close();
                    availableips.removeAt(i);
                    ips.clear();
                    lock = 0;
                    return false;
                }
               continue;
            }
        }
    }

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

void mySocktes::readyRead()
{
    QByteArray data = socket->readAll();
    writeReciever(data);
    qDebug()<<"incoming message...."+data;
    socket->close();
}


