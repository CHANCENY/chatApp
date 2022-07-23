#include "myclient.h"

myClient::myClient(QObject *parent)   : QTcpSocket{parent}
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(20);
}

void myClient::setSocket(int descriptor)
{
    // making connection through signals and slots

       socket = new QTcpSocket(this);

       connect(socket,SIGNAL(connected()),this,SLOT(connected()));
       connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
       connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

       socket->setSocketDescriptor(descriptor);

       qDebug()<<descriptor <<" connected";
}

bool myClient::WriteToFileRecievedData(QByteArray arry)
{
    QDir dir;
    QString filename = dir.absoluteFilePath("RECEIVEDDATA");
    QFile myfile(filename);
    myfile.open(QIODevice::WriteOnly|QFile::Text);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
        in<<arry;
        myfile.flush();
        myfile.close();
        return true;
    }
    return false;
}

void myClient::connected()
{
     qDebug()<<" connected...connected function";
}

void myClient::disconnected()
{
     qDebug()<<" Disconnected...";
}

void myClient::readyRead()
{
    // here you recieve data from socket connected if it has data ready

       QByteArray data = socket->readAll();

           // perform task here       

          bool OKWritten = WriteToFileRecievedData(data);

          if(OKWritten)
          {
           qDebug()<<"Data Arrived successfully .....";
           mytask *task = new mytask();
          task->setAutoDelete(true);
          connect(task,SIGNAL(Result(QByteArray)),this,SLOT(taskResult(QByteArray)),Qt::QueuedConnection);
         // pool->start(task);
          QThreadPool::globalInstance()->start(task);
          }
}

void myClient::taskResult(QByteArray handle)
{
    // sending back feedback of data to socket and to client
       // the paremeter can be change to any datatype of your choice
       //to be received after performng some task

        socket->write(handle);
        socket->close();
}
