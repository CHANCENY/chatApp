#include "contactactions.h"
#include "ui_contactactions.h"
#include <QTimer>

QTimer *review;

ContactActions::ContactActions(QWidget *parent, QString number, QString name, QTimer *t) :
    QDialog(parent),
    ui(new Ui::ContactActions)
{
    ui->setupUi(this);
    this->name = name;
    this->number = number;
    review = t;
}

ContactActions::~ContactActions()
{
    delete ui;
}

void ContactActions::on_pushButton_3_clicked()
{
    review->stop();
    QString line = "DELETE FROM contacts WHERE fullname ='"+this->name+"' AND phone ='"+this->number+"';";    
    DBFunctions del;
    if(del.deleteContactAvailable(line))
    {
        review->start(2000);
        this->close();
    }
}

void ContactActions::on_pushButton_2_clicked()
{
    review->stop();
   QString line = "SELECT * FROM contacts WHERE fullname= '"+this->name+"' AND phone='"+this->number+"';";
   DBFunctions re;
   QStringList list = re.viewDetails(line);

   if(list.isEmpty())
   {
       review->start(2000);
       return;
   }

   views = new viewDetails(this,&list);
   views->show();
   review->start(2000);

}

