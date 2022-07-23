#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QtSql>


class myDatabase
{
private:
    QSqlDatabase userdb;

public:
    myDatabase();
    void connectionSetUp();
    QSqlDatabase* getuserdb();


};

#endif // MYDATABASE_H
