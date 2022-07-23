#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>

loginpage::loginpage(QWidget *parent, QStringList *user) :
    QDialog(parent),
    ui(new Ui::loginpage)
{
    ui->setupUi(this);
    this->phone = &user->at(1);
}

loginpage::~loginpage()
{
    delete ui;
}

bool loginpage::login(QString password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM owner WHERE phone = '"+*this->phone+"';");
    if(query.exec())
    {
        while(query.next())
        {
            if(query.value(4).toString() == password)
            {
                return true;
            }
        }
    }
    return false;
}

void loginpage::on_pushButton_clicked()
{
    QString phone = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

     if(phone == nullptr && password == nullptr)
     {
         QMessageBox::warning(this,"Notification","Fill in all fields to login");
         return;
     }
    // check phone

    if(phone == *this->phone)
    {
        if(this->login(password))
        {
           DBFunctions users;
           QStringList list = users.collectOwner();

           if(users.checkSecuritySettingTwo())
           {
               QString phones = list.at(1);
               toTwo = new StepTwo(0,&phones);
               toTwo->show();
               this->close();
           }
           else
           {
               toChat = new ChatRoom(0,list.last(),list.at(1),list.at(0));
               toChat->show();
               this->close();
           }
        }
        else
        {
            QMessageBox::warning(this,"Notification","Password is invalid try again");
        }
    }
    else
    {
        QMessageBox::warning(this,"Notification",phone+ " is invalid try again");
    }
}

