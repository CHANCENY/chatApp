#include "dbfunctions.h"
#include <QDate>

DBFunctions::DBFunctions()
{

}


void DBFunctions::tablesMaker()
{

   int flag = 0;

   if(flag == 0)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE owner(fullname varchar(20), phone varchar(11), email varchar(30), profile varchar(100), password varchar(50), steptwo varchar(5), stepcode varchar(7));");
       if(query.exec())
       {
           flag = 1;

       }
   }

   if(flag == 1)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE messages(reciever varchar(11), sendernumber varchar(11),sendername varchar(40), date varchar(15), status varchar(5), message TEXT);");
       if(query.exec())
       {
           flag = 2;

       }
   }

   if(flag == 2)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE contacts(fullname varchar(20), phone varchar(11), email varchar(30), photo varchar(150));");
       if(query.exec())
       {
           flag = 3;

       }
   }

   if(flag == 3)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE pending(message TEXT, status varchar(10));");
       if(query.exec())
       {
           flag = 4;

       }
   }

   if(flag == 4)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE quick(message TEXT);");
       if(query.exec())
       {
           flag = 5;

       }

   }

   if(flag == 5)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE network(addresses varchar(20));");
       if(query.exec())
       {
            flag = 6;
       }

   }

   if(flag == 6)
   {
       QSqlQuery query;
       query.prepare("CREATE TABLE notification(status varchar(5));");
       if(query.exec())
       {
            return;
       }

   }
}

bool DBFunctions::saveUser(QStringList user)
{

   QSqlQuery query;
   query.prepare("INSERT INTO owner(fullname,phone,email,profile) VALUES('"+user[0]+"','"+user[1]+"','"+user[2]+"','"+user[3]+"');");
   if(query.exec())
   {

        return true;
   }
   return false;
}

bool DBFunctions::existingPhoneNumber(QString number)
{

  QSqlQuery query;
  query.prepare("SELECT * FROM owner WHERE phone='"+number+"';");
  if(query.exec())
  {
      while(query.next())
      {
          if(query.value(1).toString() == number)
          {
              return true;
          }
      }
  }
  return false;
}

bool DBFunctions::findingUserExist()
{
   QSqlQuery query;
   query.prepare("SELECT * FROM owner;");
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

QStringList DBFunctions::collectOwner()
{
    QStringList list;
    QSqlQuery query;
    query.prepare("SELECT * FROM owner;");
    if(query.exec())
    {
        while(query.next())
        {
            list.append(query.value(0).toString());
            list.append(query.value(1).toString());
            list.append(query.value(3).toString());
        }
    }
    return list;
}

QStringList DBFunctions::recentChat()
{
  QStringList collection, newMessages;

       QSqlQuery query;
       query.prepare("SELECT * FROM messages ORDER BY date DESC;");
       if(query.exec())
       {
           while(query.next())
           {
               collection.append(query.value(2).toString());
               this->numbers.append(query.value(1).toString());
           }
       }

       query.prepare("SELECT * FROM messages WHERE status= 'New';");
       if(query.exec())
       {
           while(query.next())
           {
               newMessages.append(query.value(2).toString());
           }
       }

       for(int i = 0; i < collection.size(); i++)
       {
           for(int j = 0; j < newMessages.size(); j++)
           {
               if(newMessages[j] == collection[i])
               {
                  QString line = collection[i]+"@New";
                  //collection.replace(i,line);
                  collection.removeAt(i);
                  collection.insert(0,line);
               }
           }
       }

       return collection;

}

QStringList DBFunctions::getNumbers()
{
    return this->numbers;
}

QString DBFunctions::getMessagesBody(QString number)
{
    QString messages;
    QSqlQuery query;
    query.prepare("SELECT * FROM messages WHERE sendernumber='"+number+"';");
    if(query.exec())
    {
        while(query.next())
        {
          messages = messages + query.value(5).toString()+"*#";
        }
    }

    return messages;
}

QString DBFunctions::getName(QString number)
{
   QSqlQuery query;
   query.prepare("SELECT sendername FROM messages WHERE sendernumber = '"+number+"';");
   if(query.exec())
   {
      while(query.next())
      {
          return query.value(0).toString();
      }
   }
   return nullptr;
}

void DBFunctions::markOldMessage(QString number)
{
  QSqlQuery query;
  query.prepare("UPDATE messages SET status = 'Old' WHERE sendernumber = '"+number+"';");
  query.exec();
  return;

}

void DBFunctions::pendingToSend(QString line)
{
  QSqlQuery query;
  query.prepare("INSERT INTO pending VALUES('"+line+"','New');");
  query.exec();
  return;
}

bool DBFunctions::saveMessagesLocal(QStringList message)
{
   qDebug()<<message;
   if(!message.isEmpty())
   {
      QSqlQuery query;
      query.prepare("SELECT * FROM messages WHERE sendernumber = '"+message[0]+"';");
      if(query.exec())
      {
        while(query.next())
        {
            if(query.value(1).toString() != nullptr)
            {
                QString m = query.value(5).toString();
                QString mbody = nullptr;
                if(m != nullptr)
                {
                    mbody = query.value(5).toString()+"*#"+message[5];
                }
                else
                {
                    mbody = message[5];
                }

               query.prepare("UPDATE messages SET status='Old', date = '"+message[3]+"', message='"+mbody+"' WHERE sendernumber = '"+message[0]+"';");
              if(query.exec())
              {
                return true;
              }
            }
            else
            {
                query.prepare("INSERT INTO messages VALUES('"+message[0]+"','"+message[1]+"','"+message[2]+"','"+message[3]+"','"+message[4]+"','"+message[5]+"');");
                if(query.exec())
                {
                    return true;
                }
            }
        }
      }
   }

   return false;
}

bool DBFunctions::deleteText(QString text, QString number, int position)
{
  if(text != nullptr && number != nullptr)
  {
      QStringList mList;
      QSqlQuery query;
      query.prepare("SELECT message FROM messages WHERE sendernumber = '"+number+"';");
      if(query.exec())
      {
          while(query.next())
          {
             mList.append(query.value(0).toString().split("*#"));
          }
      }

      QString todelete = mList[position];
      QString updated = nullptr;

      int flag = 0;
      if(todelete != nullptr)
      {
          QStringList list = todelete.split("->");
          QString line = list[0]+"\n"+list.last();

          if(line == text)
          {
              mList.removeAt(position);
              for(int i = 0; i < mList.size(); i++)
              {
                  if(i < mList.size() - 1)
                  {
                    updated = updated + mList[i] +"*#";
                  }
                  else if(i == mList.size() - 1)
                  {
                      updated = updated + mList[i];
                      flag = 1;
                  }

              }

          }

      }

      if(flag == 1)
      {
          QSqlQuery query;
          query.prepare("UPDATE messages SET message ='"+updated+"' WHERE sendernumber ='"+number+"';");
          if(query.exec())
          {
              return true;
          }
      }
  }
  return false;
}

bool DBFunctions::deleteChat(QString number)
{
    QSqlQuery query;
    QString reciever, sendernumber, sendername;
    query.prepare("SELECT * FROM messages WHERE sendernumber= '"+number+"';");
    if(query.exec())
    {
       while(query.next())
       {
           reciever = query.value(0).toString();
           sendernumber = query.value(1).toString();
           sendername = query.value(2).toString();
       }
    }

    int flag = 0;

    if(reciever != nullptr && sendernumber != nullptr && sendername != nullptr)
    {
        query.prepare("DELETE FROM messages WHERE sendernumber='"+number+"';");
        if(query.exec())
        {
           flag = 1;
        }
    }

    if(flag == 1)
    {
        query.prepare("INSERT INTO messages(reciever,sendernumber,sendername) VALUES('"+reciever+"','"+sendernumber+"','"+sendername+"');");
        if(query.exec())
        {
            return true;
        }
    }

    return false;
}

bool DBFunctions::addContacts(QString querys)
{
   QSqlQuery query;
   query.prepare(querys);
   if(query.exec())
   {
       return true;
   }

   return false;
}

bool DBFunctions::updatingContactInfo(QString querys, QString number, QString updatequery)
{

  int flag = 0;
  QString db = nullptr;

  QSqlQuery query;
  query.prepare(querys);
  if(query.exec())
  {
    flag = 1;
  }


  if(flag == 1)
  {
      query.prepare("SELECT * FROM messages WHERE sendernumber = '"+number+"';");
      if(query.exec())
      {
        while(query.next())
        {
            db = query.value(1).toString();
        }
      }
  }

  if(db != nullptr)
  {
      query.prepare(updatequery);
      if(query.exec())
      {
        return true;
      }

  }
  else
  {
      return true;
  }

  return false;

}

bool DBFunctions::deleteContactAvailable(QString querys)
{
    QSqlQuery query;
    query.prepare(querys);
    if(query.exec())
    {
        return true;
    }
    return false;
}

QStringList DBFunctions::viewDetails(QString querys)
{
    QStringList details;

  QSqlQuery query;
  query.prepare(querys);
  if(query.exec())
  {
    while(query.next())
    {
        details.append(query.value(0).toString());
        details.append(query.value(1).toString());
        details.append(query.value(2).toString());
        details.append(query.value(3).toString());
    }
  }

  return details;
}

bool DBFunctions::deleteAccount(QString line)
{
   QSqlQuery query;
   query.prepare(line);

   if(query.exec())
   {
       return true;
   }
   return false;
}

bool DBFunctions::addAddressNetwork(QStringList ip)
{
    QSqlQuery query;
    int counter = 0;

    foreach(QString line, ip)
    {
        query.prepare("insert into network values('"+line+"');");
        if(query.exec())
        {
           counter++;
        }
    }

    if(counter == ip.size())
    {
        return true;
    }

    return false;
}

QStringList DBFunctions::getIpAddress()
{
   QSqlQuery query;
   QStringList list;
   query.prepare("SELECT * FROM network;");
   if(query.exec())
   {
       while(query.next())
       {
           list.append(query.value(0).toString());
       }
   }

   return list;
}

QString DBFunctions::onActiveUpdatingChat(QString number)
{
    QString messages;
    QSqlQuery query;
    query.prepare("SELECT * FROM messages WHERE sendernumber='"+number+"' AND status = 'New';");
    if(query.exec())
    {
        while(query.next())
        {
          messages = messages + query.value(5).toString()+"*#";
        }
    }

    return messages;
}

bool DBFunctions::checkSecuritySetting()
{
  QSqlQuery query;
  query.prepare("SELECT * FROM owner;");

  if(query.exec())
  {
      while(query.next())
      {
          if(query.value(4).toString() != nullptr)
          {
              return true;
          }
      }
  }
  return false;
}

bool DBFunctions::checkSecuritySettingTwo()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM owner;");

    if(query.exec())
    {
        while(query.next())
        {
            if(query.value(5).toString() != nullptr)
            {
                return true;
            }
        }
    }
    return false;
}

QString DBFunctions::fetchAvailableIpAddressServer()
{
    QDir dir;
    QString file = dir.absoluteFilePath("storage/ipAdress.txt");

    QFile my(file);
    my.open(QIODevice::ReadOnly);
    if(my.isOpen())
    {
        QTextStream in(&my);
        QString line =in.readAll();
        my.close();
        return line;
    }
    return nullptr;
}














