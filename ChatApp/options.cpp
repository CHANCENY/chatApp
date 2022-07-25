#include "options.h"
#include "ui_options.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <QTextToSpeech>


Options::Options(QWidget *parent, QString current, int last, int row, QString number) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    // intialization

    this->currentText = current;
    this->lastIndex = last;
    this->row = row;
    this->numberPhone = number;

}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
  DBFunctions deletes;
  if(deletes.deleteText(this->currentText,this->numberPhone,this->row))
  {
      this->close();
  }
}


void Options::on_pushButton_4_clicked()
{
    DBFunctions chatdelete;
    if(chatdelete.deleteChat(this->numberPhone))
    {
        this->close();
    }
}

void Options::on_pushButton_2_clicked()
{
   QTextToSpeech *speaking = new QTextToSpeech;
   speaking->setVolume(100);
   QVoice voice;
   speaking->say(this->currentText);
   this->close();
}

