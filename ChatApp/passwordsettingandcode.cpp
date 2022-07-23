#include "passwordsettingandcode.h"
#include "ui_passwordsettingandcode.h"

passwordSettingAndCode::passwordSettingAndCode(QWidget *parent, QString number, int type) :
    QDialog(parent),
    ui(new Ui::passwordSettingAndCode)
{
    ui->setupUi(this);
    this->myNumber = number;
    this->TypeCondition = type;
    if(type == 1)
    {
       ui->password->setText("Set password");
       ui->lineEdit->setPlaceholderText("enter your password");
    }
    else if(type == 2)
    {
        ui->password->setText("Set code");
        ui->lineEdit->setPlaceholderText("enter step two code");
    }
}

passwordSettingAndCode::~passwordSettingAndCode()
{
    delete ui;
}

void passwordSettingAndCode::on_password_clicked()
{
  if(this->TypeCondition == 1)
  {
      QSqlQuery query;
      query.prepare("UPDATE owner SET password = '"+ui->lineEdit->text()+"' WHERE phone = '"+this->myNumber+"';");
      if(query.exec())
      {
          this->close();
      }
  }

  if(this->TypeCondition == 2)
  {
      QSqlQuery query;
      query.prepare("UPDATE owner SET stepcode = '"+ui->lineEdit->text()+"', steptwo = 'true' WHERE phone = '"+this->myNumber+"';");
      if(query.exec())
      {
          this->close();
      }
  }
}

