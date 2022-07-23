#include "registration.h"
#include "ui_registration.h"


Registration::Registration(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Registration)
{
    ui->setupUi(this);

    DBFunctions con;
    con.connectionSetUp();
    ui->email->setEnabled(false);
    ui->name->setEnabled(false);
    ui->phone->setFocus();

    QDir dir;
    dir.absolutePath();
    QStringList di = dir.entryList();

    int counter = 0;

    foreach(QString l, di)
    {
        counter++;
        if(l == "PicturesUploaded")
        {
            break;
        }
        if(l != "PicturesUploaded" && counter == di.size())
        {
            dir.mkdir("PicturesUploaded");
            break;
        }

    }



}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_phone_returnPressed()
{
    QString phone = ui->phone->text();

    // check for non figure characters

    if(phone.size() == 10)
    {
      ui->phone->setStyleSheet("background-color:white");

      for(int i = 0; i < phone.size(); i++ )
      {
          if(phone[i] == '0' || phone[i] == '1' || phone[i] == '2' || phone[i] == '3' || phone[i] == '4' || phone[i] == '5' || phone[i] == '6' || phone[i] == '7' || phone[i] == '8' || phone[i] == '9')
          {
              this->phoneNumber = phone;
              ui->email->setEnabled(true);
              ui->email->setFocus();
              ui->phone->setStyleSheet("background-color:white;");
              ui->phone->setToolTip("<font color='green'>phone is valid </font>");
          }
          else
          {
              ui->phone->setStyleSheet("background-color:red;");
              ui->phone->setToolTip("All characters should be numbers");
          }
      }

    }
    else
    {
        ui->phone->setStyleSheet("background-color:red");
        ui->phone->setToolTip("phone max length should be 10");
    }

}

void Registration::on_email_returnPressed()
{
    QString email = ui->email->text();

    int sizes = email.size();

    for(int i = 0; i < sizes; i++)
    {
        if(email[i] == '@' &&  i < sizes)
        {
            ui->email->setStyleSheet("background-color:white;");
            ui->email->setToolTip("<font color='green'>email is valid </font>");
            this->emailAddress = email;
            ui->name->setEnabled(true);
            ui->name->setFocus();
            break;
        }
        else
        {
            ui->email->setStyleSheet("background-color:red;");
            ui->email->setToolTip("Invalid email should contain '@'");
        }
    }
}

void Registration::on_name_returnPressed()
{
    QString names = ui->name->text();

    // check for non figure characters

    for(int i = 0; i < names.size(); i++ )
    {
        if(names[i] != '0' && names[i] != '1' && names[i] != '2' && names[i] != '3' && names[i] != '4' && names[i] != '5' && names[i] != '6' && names[i] != '7' && names[i] != '8' && names[i] != '9')
        {
            this->fullName = names;

            ui->name->setStyleSheet("background-color:white;");
            ui->name->setToolTip("<font color='green'>name is valid </font>");
        }
        else
        {
            ui->name->setStyleSheet("background-color:red;");
            ui->name->setToolTip("All characters should not be numbers");
        }
    }

    if(this->fullName != nullptr && this->phoneNumber != nullptr && this->emailAddress != nullptr)
    {

        imagesupload = new profileUpload(0, &this->fullName,&this->phoneNumber, &this->emailAddress);
        imagesupload->show();
        this->close();
    }
}

