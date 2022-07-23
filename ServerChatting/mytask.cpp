#include "mytask.h"


mytask::mytask()
{

}

void mytask::TaskGivenFile()
{
    QDir dir;
    QString filename = dir.absoluteFilePath("RECEIVEDDATA");
    QFile myfile(filename);
    myfile.open(QIODevice::ReadOnly);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
       this->data = in.readAll().toUtf8();
        myfile.close();
    }

}

void mytask::PassDataTosend(QByteArray arr)
{
    QDir dir;
    QString filename = dir.absoluteFilePath("RECEIVEDDATA");
    QFile myfile(filename);
    myfile.open(QIODevice::WriteOnly|QFile::Text);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
         in<<arr;
         myfile.flush();
        myfile.close();
    }
}

QByteArray mytask::Sending()
{
    QDir dir;
    QString filename = dir.absoluteFilePath("RECEIVEDDATA");
    QFile myfile(filename);
    myfile.open(QIODevice::ReadOnly);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
       QByteArray r = in.readAll().toUtf8();
        myfile.close();
        return r;
    }
    return nullptr;
}

void mytask::DecisionPath()
{
    QString dataline =QString::fromStdString(this->data.toStdString());
    QStringList list = dataline.split('@');


    // action variable hold the request type to be done eg login
    QString action = list.at(0);

    if(action == "sending")
    {
      this->PassDataTosend(saveSentMessages(list.last()).toLocal8Bit());
    }
    if(action == "newmessages")
    {
        this->PassDataTosend(sendnewMessages(list.last()).toLocal8Bit());
    }
    if(action == "pendingmessages")
    {
        this->PassDataTosend(saviPendingMessages(list.last()).toLocal8Bit());
    }
    if(action == "changeAccountInfo")
    {
       this->PassDataTosend(changeAccountInfo(list.last()).toLocal8Bit());
    }


}

void mytask::run()
{
    qDebug()<<"Task has started.....";

    // to processing request
       TaskGivenFile();
       DecisionPath();

       // after processing request and sending feedback

      QByteArray data = Sending();
       qDebug()<<"Task done....";
       qDebug()<<"Sending result......";
       emit Result(data);
}
