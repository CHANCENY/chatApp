#include "interclass.h"

interClass::interClass()
{

}

void interClass::connection()
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("DATABASE.db");
    mydb.open();

}

QString interClass::saveSentMessages(QString messages)
{ 
   this->connection();
   if(messages != nullptr)
   {
      QSqlQuery query;
      query.prepare(messages);
      if(query.exec())
      {
          return "true";
      }
   }
   return "false";
}

QString interClass::sendnewMessages(QString number)
{
 this->connection();
  QString sender = nullptr;
  QString line;
  QString body;
  QSqlQuery query;
  query.prepare("SELECT * FROM messages WHERE reciever = '"+number+"' AND status = 'New' LIMIT 1;");
  if(query.exec())
  {
      while(query.next())
      {
          sender = query.value(1).toString();
      }
  }

  if(sender != nullptr)
  {
      query.prepare("SELECT * FROM messages WHERE reciever ='"+number+"' AND status = 'New' LIMIT 1;");
      if(query.exec())
      {
          while(query.next())
          {
              body = query.value(5).toString();
              QString messagebody =query.value(3).toString()+"->rec->"+query.value(5).toString();
              line = query.value(0).toString()+"#->"+query.value(1).toString()+"#->"+query.value(2).toString()+"#->"+query.value(3).toString()+"#->"+query.value(4).toString()+"#->"+messagebody+"@";

          }
      }
  }


 if(line != nullptr)
 {
     query.prepare("UPDATE messages SET status ='Old' WHERE sendernumber = '"+sender+"' AND message ='"+body+"';");
     query.exec();
     return line;
 }

 return "false";

}

QString interClass::saviPendingMessages(QString message)
{
   this->connection();
  if(message != nullptr)
  {
      QStringList mList = message.split("%%%");
      int counter = 0;
      for(int i = 0; i < mList.size() - 1; i++)
      {
          QSqlQuery query;
          query.prepare(mList[i]);
          if(query.exec())
          {
              counter++;
          }
      }

      if(counter == mList.size() - 1)
      {
          return "true";
      }
  }
  return "false";
}

QString interClass::changeAccountInfo(QString line)
{
  this->connection();
  QSqlQuery query;
  query.prepare(line);
  if(query.exec())
  {
      return "true";
  }
  return "false";
}
