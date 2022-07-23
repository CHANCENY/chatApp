#include "mydatabase.h"

myDatabase::myDatabase()
{

}

void myDatabase::connectionSetUp()
{
    userdb = QSqlDatabase::addDatabase("QSQLITE");
    userdb.setDatabaseName("DATABASE.db");
    userdb.open();

    qDebug()<<"connected..";
}

QSqlDatabase* myDatabase::getuserdb()
{
    return &userdb;
}


