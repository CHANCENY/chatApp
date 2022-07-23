#ifndef INTERCLASS_H
#define INTERCLASS_H

#include <QString>
#include <QtSql>


class interClass
{
public:
    interClass();
    void connection();
    QString saveSentMessages(QString messages);
    QString sendnewMessages(QString number);
    QString saviPendingMessages(QString message);
    QString changeAccountInfo(QString line);

private:
     QSqlDatabase mydb;
};

#endif // INTERCLASS_H
