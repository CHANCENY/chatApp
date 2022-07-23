#include "steptwo.h"
#include "ui_steptwo.h"
#include <QStringList>

StepTwo::StepTwo(QWidget *parent,QString *phone) :
    QDialog(parent),
    ui(new Ui::StepTwo)
{
    ui->setupUi(this);
    this->phone = phone;
}

StepTwo::~StepTwo()
{
    delete ui;
}

bool StepTwo::stepTwoVerification(QString code)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM owner;");

    if(query.exec())
    {
        while(query.next())
        {
            if(query.value(6).toString() == code)
            {
                return true;
            }
        }
    }
    return false;
}

void StepTwo::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1 != nullptr)
    {
        if(this->stepTwoVerification(arg1))
        {
            DBFunctions two;
            QStringList userdetails = two.collectOwner();
            room = new ChatRoom(0,userdetails.last(),userdetails.at(1),userdetails.at(0));
            room->show();
            this->close();
        }
        else
        {
            ui->lineEdit->setStyleSheet("background-color:red;");
        }
    }
    ui->lineEdit->setStyleSheet("background-color:white;");
}

