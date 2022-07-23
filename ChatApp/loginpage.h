#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QtSql>
#include "steptwo.h"
#include "chatroom.h"
#include "dbfunctions.h"

namespace Ui {
class loginpage;
}

class loginpage : public QDialog
{
    Q_OBJECT

public:
    explicit loginpage(QWidget *parent = nullptr, QStringList *user = nullptr);
    ~loginpage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::loginpage *ui;
    const QString *phone;
    StepTwo *toTwo;
    ChatRoom *toChat;


    bool login(QString password);

};

#endif // LOGINPAGE_H
