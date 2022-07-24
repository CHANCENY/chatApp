#include "mydatabase.h"

myDatabase::myDatabase()
{

}

void myDatabase::connectionSetUp()
{
    QDir dir;
    dir.mkdir("storage");
    userdb = QSqlDatabase::addDatabase("QSQLITE");
    userdb.setDatabaseName(dir.currentPath()+"/storage/DATABASE.db");
    userdb.open();

    qDebug()<<"connected..";
}

QSqlDatabase* myDatabase::getuserdb()
{
    return &userdb;
}


