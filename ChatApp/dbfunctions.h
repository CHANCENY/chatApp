#ifndef DBFUNCTIONS_H
#define DBFUNCTIONS_H

#include <QtSql>
#include <QString>
#include <QStringList>
#include "mydatabase.h"




class DBFunctions: public myDatabase
{
private:
    QStringList numbers;
public:
    DBFunctions();


    // making database tables
    void tablesMaker();

    // registering user here
    bool saveUser(QStringList user);
    bool existingPhoneNumber(QString number);
    bool findingUserExist();
    QStringList collectOwner();
    QStringList recentChat();
    QStringList getNumbers();
    QString getMessagesBody(QString number);
    QString getName(QString number);
    void markOldMessage(QString number);
    void pendingToSend(QString line);
    bool saveMessagesLocal(QStringList message);

    bool deleteText(QString text, QString number, int position);
    bool deleteChat(QString number);
    bool addContacts(QString querys);
    bool updatingContactInfo(QString querys, QString number, QString updatequery);
    bool deleteContactAvailable(QString querys);
    QStringList viewDetails(QString querys);
    bool deleteAccount(QString line);
    bool addAddressNetwork(QStringList ip);
    QStringList getIpAddress();
    QString onActiveUpdatingChat(QString number);
    bool checkSecuritySetting();
    bool checkSecuritySettingTwo();

};

#endif // DBFUNCTIONS_H
