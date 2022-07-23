#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QObject>
#include <QString>
#include "myclient.h"
#include <QFile>
#include <QTextStream>
#include <QDir>


class myServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit myServer(QObject *parent = 0);
       QString startServer();
       QString portFinder();
       bool makeTables();

protected:
    void incomingConnection(qintptr handle);

private:

};

#endif // MYSERVER_H
