#include "editcontact.h"
#include "ui_editcontact.h"
#include <QFileDialog>
#include <QMessageBox>


QTimer *TT1, *TT2, *TT3, *TT4;

QString fileprofile = nullptr;
QString phonefoundindb = nullptr;

editContact::editContact(QWidget *parent, QTimer *t1, QTimer *t2, QTimer *t3, QTimer *t4) :
    QDialog(parent),
    ui(new Ui::editContact)
{
    ui->setupUi(this);
    TT1 = t1; TT2 = t2; TT3 = t3; TT4 = t4;
}

editContact::~editContact()
{
    delete ui;
}

void editContact::fetchContact(QString line)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM contacts WHERE fullname ='"+line+"' OR phone = '"+line+"' OR email= '"+line+"' OR photo = '"+line+"';");
    if(query.exec())
    {
        while(query.next())
        {
            ui->fullname->setText(query.value(0).toString());
            ui->phonenumber->setText(query.value(1).toString());
            ui->email->setText(query.value(2).toString());
            phonefoundindb = query.value(1).toString();

            if(query.value(3).toString() == nullptr)
            {
                QPixmap pro(":/ic/icons/user.png");
                ui->profilelabel->setPixmap(pro);
            }
            else
            {
                fileprofile = query.value(3).toString();
                QPixmap pro(query.value(3).toString());
                ui->profilelabel->setPixmap(pro);
            }

        }
    }
}

bool editContact::uniqueChecker(QString values, QString type)
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

bool editContact::processingImage(QString file)
{

    if(file == nullptr)
    {
        return false;
    }

    QByteArray dataimage;

    QFile myfile(file);
    myfile.open(QIODevice::ReadOnly);
    if(myfile.isOpen())
    {
        dataimage = myfile.readAll();
        myfile.close();
    }

    if(dataimage.isEmpty())
    {
        return false;
    }

    QFileInfo info(file);

    QString target = "PicturesUploaded/"+info.fileName();


    QFile myfiles(target);
    myfiles.open(QIODevice::WriteOnly|QFile::NewOnly);
    if(myfiles.isOpen())
    {

       myfiles.write(dataimage);
       myfiles.flush();
       myfiles.close();

       fileprofile = target;
       return true;
    }

    return false;

}

void editContact::on_uploadp_clicked()
{
   QString filename = QFileDialog::getOpenFileName(this,"Choose photo",QDir::homePath(),"*.jpg; *.jpeg; *.png");
   qDebug()<<filename;
   if(filename != nullptr)
   {
       QFileInfo info(filename);

       if(info.size() < 2000000)
       {
         if(processingImage(filename))
         {
             QPixmap pro(fileprofile);
             ui->profilelabel->setPixmap(pro);
         }
         else
         {
             QMessageBox::information(this,"information",info.fileName()+" failed to upload!");
         }

       }
       else
       {
          QMessageBox::warning(0,"Size warning","This file size is ("+QString::number(+info.size() / 1000000)+"MB) You file is too big (2MB) max is allowed!");
       }
   }
}

void editContact::on_fullname_textChanged(const QString &arg1)
{
    if(arg1 == nullptr)
    {
        return;
    }

    this->fetchContact(arg1);
}


void editContact::on_phonenumber_textChanged(const QString &arg1)
{
    if(arg1 == nullptr)
    {
        return;
    }

    this->fetchContact(arg1);

}


void editContact::on_email_textChanged(const QString &arg1)
{
    if(arg1 == nullptr)
    {
        return;
    }

    this->fetchContact(arg1);
}


void editContact::on_addcontact_clicked()
{
    TT1->stop(); TT2->stop(); TT3->stop(); TT4->stop();
    QString name = ui->fullname->text();
    QString number = ui->phonenumber->text();
    QString email = ui->email->text();

    if(name != nullptr && number != nullptr)
    {
       QString line = "UPDATE contacts SET fullname = '"+name+"', phone = '"+number+"', email='"+email+"', photo ='"+fileprofile+"' WHERE phone = '"+phonefoundindb+"';";
       QString line2 = "UPDATE messages SET sendernumber ='"+number+"', sendername = '"+name+"' WHERE sendernumber= '"+phonefoundindb+"';";
       DBFunctions update;
       if(update.updatingContactInfo(line,phonefoundindb,line2))
       {
          TT3->start(2000);
          TT2->start(5000);
          TT1->start(7200);
          TT4->start(2000);
          this->close();
       }
    }
}

