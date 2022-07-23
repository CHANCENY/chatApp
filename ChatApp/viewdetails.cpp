#include "viewdetails.h"
#include "ui_viewdetails.h"

viewDetails::viewDetails(QWidget *parent, QStringList *l) :
    QDialog(parent),
    ui(new Ui::viewDetails)
{
    ui->setupUi(this);

    QStringList list = *l;

    if(!list.isEmpty())
    {
        ui->name->setText("Name: "+list.first());
        ui->phone->setText("Phone: "+list.at(1));
        ui->email->setText("Email: "+list.at(2));

        if(list.last() != nullptr)
        {
            QPixmap image(list.last());
            ui->image->setPixmap(image);
        }
    }
}

viewDetails::~viewDetails()
{
    delete ui;
}
