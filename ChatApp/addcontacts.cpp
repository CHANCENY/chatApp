#include "addcontacts.h"
#include "ui_addcontacts.h"
#include <QDir>
#include <QMessageBox>
#include <QtSql>



QString target =nullptr;
QString fullnamecontact = nullptr;
QString phonecontact = nullptr;
QString emailcontact = nullptr;

QTimer *T1, *T2, *T3, *T4;


AddContacts::AddContacts(QWidget *parent, QTimer *t1, QTimer *t2, QTimer *t3, QTimer *t4) :
    QDialog(parent),
    ui(new Ui::AddContacts)
{
    ui->setupUi(this);
    T1 = t1; T2 = t2; T3 = t3;
    T4 = t4;

}

AddContacts::~AddContacts()
{
    delete ui;
}

bool AddContacts::in_array(QString hint, QStringList list)
{
    if(hint != nullptr && !list.isEmpty())
    {
        foreach(QString line, list)
        {
            if(line == hint)
            {
                return true;
            }
        }
    }
    return false;
}

bool AddContacts::move_file_upload(QByteArray filename, QString destination)
{
    if(filename != nullptr && destination != nullptr)
    {
        QDir dir;
        QString file = dir.absoluteFilePath(destination);

        QFile myfile(file);
        myfile.open(QIODevice::WriteOnly|QFile::NewOnly);
        if(myfile.isOpen())
        {
            myfile.write(filename);
            myfile.flush();
            myfile.close();
            return true;
        }
    }
    return false;
}

bool AddContacts::uniqueCotactCheck(QString values, QString type)
{
   if(type == "name")
   {
       QSqlQuery query;
       query.prepare("SELECT * FROM contacts WHERE fullname ='"+values+"';");
       if(query.exec())
       {
           while(query.next())
           {
               if(query.value(0).toString() != nullptr)
               {
                   return false;
               }
           }
       }
   }
   else if(type == "phone")
   {
       QSqlQuery query;
       query.prepare("SELECT * FROM contacts WHERE phone ='"+values+"';");
       if(query.exec())
       {
           while(query.next())
           {
               if(query.value(0).toString() != nullptr)
               {
                   return false;
               }
           }
       }
   }

   return true;
}


void AddContacts::on_uploadp_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Choose photo",QDir::homePath(),"*.jpg *.jpeg *.png");
    if(filename != nullptr)
    {
        QFileInfo info(filename);

        if(info.size() < 2000000)
        {
           QString extension = info.fileName().split('.').last();
           QStringList list = {"jpg","jpeg","png"};

           if(in_array(extension, list))
           {
               QByteArray filedata;

               QFile myfile(filename);
               myfile.open(QIODevice::ReadOnly);
               if(myfile.isOpen())
               {
                  filedata = myfile.readAll();
                  myfile.close();
               }

               if(!filedata.isEmpty())
               {
                    target = "PicturesUploaded/"+info.fileName();
                   if(move_file_upload(filedata,target))
                   {
                      QPixmap pro(target);
                      ui->profilelabel->setPixmap(pro);
                   }
               }
           }
           else
           {
               QMessageBox::warning(0,"Type warning","This file ("+extension+") is not allowed!");
           }

        }
        else
        {
           QMessageBox::warning(0,"Size warning","This file size is ("+QString::number(+info.size() / 1000000)+"MB) You file is too big (2MB) max is allowed!");
        }

    }
}


void AddContacts::on_addcontact_clicked()
{
  T1->stop(); T2->stop(); T3->stop(); T4->stop();

  if(fullnamecontact != nullptr && phonecontact != nullptr)
  {
      QString line = "INSERT INTO contacts(fullname,phone,email,photo) VALUES('"+fullnamecontact+"','"+phonecontact+"','"+emailcontact+"','"+target+"');";
      DBFunctions addnow;

      if(this->uniqueCotactCheck(fullnamecontact,"name"))
      {
          if(this->uniqueCotactCheck(phonecontact,"phone"))
          {
              if(addnow.addContacts(line))
              {
                  T4->start(2000);
                  T3->start(2000);
                  T2->start(5000);
                  T1->start(7200);
                  this->close();
              }
          }
          else
          {
              QMessageBox::warning(this,"warning","Your number already exist");
              T4->start(2000);
              T3->start(2000);
              T2->start(5000);
              T1->start(7200);
          }
      }
      else
      {
          QMessageBox::warning(this,"warning","Your name already exist");
          T4->start(2000);
          T3->start(2000);
          T2->start(5000);
          T1->start(7200);
      }
  }
}


void AddContacts::on_email_textChanged(const QString &arg1)
{
    QString emails = arg1;
    int flag = 0;

    if(emails == nullptr)
    {
        return;
    }


    for(int i = 0; i <emails.size(); i++)
    {
        if(emails[i]  == '@' && i < emails.size())
        {
            ui->email->setStyleSheet("background-color:white;");
            ui->email->setToolTip("<font color='green'>Email is valid</font>");
            flag = 2;
            break;
        }
        else
        {
            ui->email->setStyleSheet("background-color:red;");
            ui->email->setToolTip("Email is invalid");

            flag = 0;
        }
    }

    if(flag == 2)
    {
      emailcontact = emails;
    }
}


void AddContacts::on_fullname_textChanged(const QString &arg1)
{
    QString name = arg1;
    int flag = 0;

    if(name == nullptr)
    {
        return;
    }

    for(int i = 0; i < name.size(); i++)
    {
        if(name[i] == '0' || name[i] == '1' || name[i] == '2' || name[i] == '3' || name[i] == '4' || name[i] == '5' || name[i] == '6' || name[i] == '7' || name[i] == '8' || name[i] == '9')
        {
            ui->fullname->setStyleSheet("background-color:red;");
            ui->fullname->setToolTip("Name should not contain any number");
            flag = 0;

        }
        else
        {
            flag =2;
        }
    }

    if(uniqueCotactCheck(name,"name") && flag == 2)
    {
       fullnamecontact = name;
       ui->fullname->setStyleSheet("background-color:white;");
       ui->fullname->setToolTip("<font color='green'>Name is valid</font>");
    }

}


void AddContacts::on_phonenumber_textChanged(const QString &arg1)
{
    QString phone = arg1;
    int flag = 0;

    if(phone == nullptr)
    {
        return;
    }


    if(phone.size() == 10)
    {
        for(int i = 0; i < phone.size(); i++)
        {
            if(phone[i] == '0' || phone[i] == '1' || phone[i] == '2' || phone[i] == '3' || phone[i] == '4' || phone[i] == '5' || phone[i] == '6' || phone[i] == '7' || phone[i] == '8' || phone[i] == '9')
            {
                ui->phonenumber->setStyleSheet("background-color:white;");
                ui->phonenumber->setToolTip("<font color='green'>Phone number is valid</font>");
                flag = 2;
            }
            else
            {
                flag = 0;
                ui->phonenumber->setStyleSheet("background-color:red;");
                ui->phonenumber->setToolTip("phone number should not contain any letter");

            }
        }

        if(uniqueCotactCheck(phone,"phone") && flag == 2)
        {

           phonecontact = phone;

        }
    }
    else
    {
        ui->phonenumber->setStyleSheet("background-color:red;");
        ui->phonenumber->setToolTip("phone number should have 10 digits");
    }
}

