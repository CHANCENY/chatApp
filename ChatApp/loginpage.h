#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QtSql>
#include "steptwo.h"
#include "chatroom.h"
#include "dbfunctions.h"
#include "forgotpassword.h"

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

    void on_pushButton_2_clicked();

private:
    Ui::loginpage *ui;
    const QString *phone;
    StepTwo *toTwo;
    ChatRoom *toChat;
    forgotpassword *forgot;


    bool login(QString password);

};

#endif // LOGINPAGE_H
