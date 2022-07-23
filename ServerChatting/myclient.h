#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QThreadPool>
#include <QFile>
#include <QTextStream>
#include "mytask.h"
#include <QDir>

class myClient : public QTcpSocket
{
    Q_OBJECT
public:    
    explicit myClient(QObject *parent = nullptr);
       void setSocket(int descriptor);

       bool WriteToFileRecievedData(QByteArray arry);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void taskResult(QByteArray handle);


signals:


private:
    QTcpSocket *socket;
    QThreadPool *pool;
};

#endif // MYCLIENT_H
