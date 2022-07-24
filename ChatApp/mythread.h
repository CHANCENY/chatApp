#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QtCore>
#include <QNetworkInterface>
#include <QtSql>




class myThread : public QThread
{
public:
    myThread();
    void run();
    void connection();
    void collectingIps();
    bool settingNetworkCheck();
    void flagging(int x);
    QString ipFound();
    QString getFoundIp();
    void collectall(QStringList list);

private:
    int flag;
    QString ipAddress;
    QStringList ips;
};

#endif // MYTHREAD_H
