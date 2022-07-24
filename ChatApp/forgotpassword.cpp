#include "forgotpassword.h"
#include "ui_forgotpassword.h"
#include <QtSql>
#include <QMessageBox>
#include <QLineEdit>
#include <QRandomGenerator>


static int lock = 0;

static int changeway= 0;
QString codemade;

forgotpassword::forgotpassword(QWidget *parent, QString phone) :
    QDialog(parent),
    ui(new Ui::forgotpassword)
{
    ui->setupUi(this);
    this->myPhoneNumber = phone;
    QString missing = "******";
    QStringList list; list.append(this->myPhoneNumber[0]);
    list.append(this->myPhoneNumber[1]);
    list.append(missing);
    list.append(this->myPhoneNumber[8]);
    list.append(this->myPhoneNumber[9]);
    QString number  = nullptr;
    foreach(QString line, list)
    {
       number = number+line;
    }
    ui->instruction->setText("Give digits missing in this number "+number);
    ui->pushButton->setText("Check digits");
}

forgotpassword::~forgotpassword()
{
    delete ui;
}

void forgotpassword::sendCode()
{

}

bool forgotpassword::checkDigits(QString digits)
{
   QString extracted = nullptr;

   for(int i = 0; i < this->myPhoneNumber.length(); i++)
   {
       if(i >1 && i < 8)
       {
           extracted = extracted + this->myPhoneNumber[i];
       }
   }

   if(extracted == digits)
   {
       return true;
   }
   return false;
}

bool forgotpassword::change(QString nepassword)
{
  QSqlQuery query;
  query.prepare("UPDATE owner SET password ='"+nepassword+"' WHERE phone = '"+this->myPhoneNumber+"';");
  if(query.exec())
  {
      return true;
  }
  return false;
}

void forgotpassword::messagePopUp(QString message, QString title)
{
    QSystemTrayIcon *pop = new QSystemTrayIcon(this);
    pop->setIcon(QIcon(":/ic/icons/chat.png"));
    pop->setVisible(true);
    pop->showMessage(title,message,QIcon(":/ic/icons/chat.png"),10000);
}

void forgotpassword::on_pushButton_clicked()
{
    if(lock == 0)
    {
        QString digits = ui->lineEdit->text();
       if(this->checkDigits(digits))
       {
           ui->lineEdit->setPlaceholderText("enter new password");
           ui->lineEdit->clear();
           ui->instruction->setText("Enter your new password below!");
           ui->lineEdit->setEchoMode(QLineEdit::Password);
           ui->pushButton->setText("Change");
           lock = 1;
       }
       else
       {
         messagePopUp("This is wrong completion digits try again","Error Notification");
       }
    }
    else if(lock == 1)
    {
       QString newpassword = ui->lineEdit->text();
       if(this->change(newpassword))
       {
           messagePopUp("Password Changed successfully","Change Notification");
           this->close();
       }
       else
       {
           messagePopUp("Connection error problem occurred","Network error Notification");
       }
    }
    else if(lock == 3)
    {
        QString line = ui->lineEdit->text();
        if(line == codemade)
        {
            ui->lineEdit->setPlaceholderText("enter new password");
            ui->lineEdit->clear();
            ui->instruction->setText("Enter your new password below!");
            ui->lineEdit->setEchoMode(QLineEdit::Password);
            ui->pushButton->setText("Change");
            lock = 1;
        }
        else
        {
            messagePopUp("Invalid code try gain","Error Notification");
        }
    }
}


void forgotpassword::on_pushButton_2_clicked()
{
    codemade.clear();
    if(changeway == 0)
    {
        ui->lineEdit->setPlaceholderText("enter sent code");
        ui->pushButton->setText("Enter code");
        ui->instruction->setText("Enter sent code below");
        lock = 3;
        changeway = 1;

        for(int i = 0; i < 7; i++)
        {
          codemade = codemade + QString::number(QRandomGenerator::global()->bounded(0,9));
        }
        this->messagePopUp("Reset Code "+codemade,"Resetting Code notification");
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        QString missing = "******";
        QStringList list; list.append(this->myPhoneNumber[0]);
        list.append(this->myPhoneNumber[1]);
        list.append(missing);
        list.append(this->myPhoneNumber[8]);
        list.append(this->myPhoneNumber[9]);
        QString number  = nullptr;
        foreach(QString line, list)
        {
           number = number+line;
        }
        ui->instruction->setText("Give digits missing in this number "+number);
        ui->pushButton->setText("Check digits");
        lock = 0;
        changeway = 0;
    }
}

