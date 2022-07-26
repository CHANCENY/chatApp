#include "myserver.h"
#include <QtSql>

myServer::myServer(QObject *parent) :  QTcpServer(parent)
{

}

QString myServer::startServer()
{

    //int port = portFinder().toInt();

    if(!this->listen(QHostAddress::Any,8889))
    {
        qDebug()<<"server not listening ....";
        return nullptr;
    }
    else
    {
          qDebug()<<"----SERVER LISTENING AT PORT "<<8889;
          return "true";
    }
     return nullptr;

}

QString myServer::portFinder()
{
  QDir dir;
  QString filename = dir.absoluteFilePath("PORT");

  QFile myfile(filename);
  myfile.open(QIODevice::ReadOnly);
  if(myfile.isOpen())
  {
      QTextStream in(&myfile);
      return in.readAll();
  }
  return nullptr;
}

bool myServer::makeTables()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("DATABASE.db");
  if(db.open())
  {
      QSqlQuery query;

      query.prepare("SELECT * FROM messages;");
      if(query.exec())
      {
         return true;
      }
      query.prepare("CREATE TABLE messages(reciever varchar(11), sendernumber varchar(11),sendername varchar(40), date varchar(15), status varchar(5), message TEXT);");
      if(query.exec())
      {
          db.close();
          db = QSqlDatabase();
          db.removeDatabase("DATABASE.db");
          return true;
      }
  }
  return false;
}

void myServer::incomingConnection(qintptr handle)
{
    // this vrtual function has been overrider to handle incoming
       // connection

       myClient *client = new myClient(this);
       client->setSocket(handle);


       qDebug()<<handle<<" connection established successfully....";
}

