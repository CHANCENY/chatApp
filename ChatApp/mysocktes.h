#ifndef MYSOCKTES_H
#define MYSOCKTES_H

#include <QTcpSocket>
#include <QObject>
#include "dbfunctions.h"
#include <QHostAddress>
#include <QHostInfo>

class mySocktes : public QTcpSocket
{
    Q_OBJECT

private:
    QTcpSocket *socket;

public:
   explicit mySocktes(QObject *parent = nullptr);
    bool sendingMessages(QByteArray mData);
    void writeReciever(QByteArray data);


public slots:
    void readyRead();


};

#endif // MYSOCKTES_H
