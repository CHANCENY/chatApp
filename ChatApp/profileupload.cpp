#include "profileupload.h"
#include "ui_profileupload.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "mythread.h"



profileUpload::profileUpload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::profileUpload)
{
    ui->setupUi(this);
    this->checkedAgreement = 0;

    //checking and making table

    DBFunctions tables;
    tables.tablesMaker();

}

profileUpload::profileUpload(QWidget *parent, QString *fullName, QString *phoneNumber, QString *emailAddress):
    QDialog(parent),
    ui(new Ui::profileUpload)
{
    ui->setupUi(this);
    this->fullName = *fullName;
    this->phoneNumber = *phoneNumber;
    this->emailAddress = *emailAddress;

    DBFunctions tables;
    tables.tablesMaker();

}

profileUpload::~profileUpload()
{
    delete ui;
}

bool profileUpload::in_Array(QString hint, QStringList stacks)
{
    if(!stacks.isEmpty() && hint != nullptr)
    {
       foreach(QString line, stacks)
       {
           if(line == hint)
           {
               return true;
           }
       }
    }
    return false;
}

bool profileUpload::move_file_upload(QByteArray filedata, QString destination)
{
  if(filedata!= nullptr && destination !=nullptr)
  {
      QFile myfile(destination);
      myfile.open(QIODevice::WriteOnly|QFile::NewOnly);
      if(myfile.isOpen())
      {
          myfile.write(filedata);
          myfile.flush();
          myfile.close();
          return true;
      }
  }
  return false;
}

void profileUpload::on_pushButton_2_clicked()
{

    if(this->fullName != nullptr && this->emailAddress != nullptr && this->phoneNumber != nullptr && this->checkedAgreement == 2 && !this->imageProfile.isEmpty())
    {

        QString destination = "PicturesUploaded/"+ui->imageuploaded->text();

        // moving image

        if(move_file_upload(this->imageProfile,destination))
        {
            // sending to server for storage reference

            QStringList usermade;
            usermade.append(this->fullName);
            usermade.append(this->phoneNumber);
            usermade.append(this->emailAddress);
            usermade.append(destination);

            DBFunctions reg;

            if(reg.existingPhoneNumber(this->phoneNumber))
            {
                QMessageBox::warning(this,"warning","this "+this->phoneNumber+" already exist!");
                return;
            }
            else
            {
                if(reg.saveUser(usermade))
                {
                    room = new ChatRoom(0,destination,this->phoneNumber,this->fullName);
                    room->show();
                    this->close();
                }
            }
        }
    }

    // room = new ChatRoom(this);
    // room->show();

}

void profileUpload::on_pushButton_clicked()
{
    QString imageselected = QFileDialog::getOpenFileName(this,"choose profile",QDir::homePath(),"Image *.jpg; *.jpeg; *.png");

    if(imageselected != nullptr)
    {
        QFileInfo info(imageselected);
        ui->imageuploaded->setText(info.fileName());
        if(info.size() < 2000000)
        {
           QStringList list = info.fileName().split('.');
           QStringList ext = {"jpg","jpeg","png"};

           // check extension

           if(in_Array(list.last(),ext))
           {
             QFile myfile(imageselected);
             myfile.open(QIODevice::ReadOnly);
             if(myfile.isOpen())
             {
                 this->imageProfile = myfile.readAll();
                 myfile.close();
             }
           }
           else
           {
              QMessageBox::warning(this,"warning","The file Type is allowed try (jpg,jpeg,png)");
           }
        }
        else
        {
           QMessageBox::warning(this,"warning","The file size is too big max 2 mb");
        }



    }
}

void profileUpload::on_checkBox_stateChanged(int arg1)
{
    this->checkedAgreement = arg1;

}

