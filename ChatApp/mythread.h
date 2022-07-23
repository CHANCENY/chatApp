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
};

#endif // MYTHREAD_H
