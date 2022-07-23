#include "networkdilog.h"
#include "ui_networkdilog.h"
#include <QTimer>


 QString Line ="please wait for moment";
 QString dot = "..";
 static int counts = 0;
 QTimer *timer;

networkDilog::networkDilog(QWidget *parent, QThread *thread) :
    QDialog(parent),
    ui(new Ui::networkDilog)
{
    ui->setupUi(this);

    this->threadin = thread;

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(changing()));
    timer->start(1000);

}

networkDilog::~networkDilog()
{
    delete ui;
}

void networkDilog::changing()
{
    ui->label_2->setText(Line+dot);
    if(counts < 5)
    {
     dot = dot +".";
     ui->label_2->setText(Line+dot);
     counts++;
    }

    if(counts == 5)
    {
        dot = ".";
        ui->label_2->setText(Line+dot);
        counts = 0;
    }

    if(this->threadin->isFinished())
    {
        qDebug()<<"thread finished";
        timer->stop();

        //go registration page
        regirs = new Registration(0);
        regirs->show();
        this->close();
    }


}
