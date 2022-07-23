#ifndef VIEWDETAILS_H
#define VIEWDETAILS_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class viewDetails;
}

class viewDetails : public QDialog
{
    Q_OBJECT

public:
    explicit viewDetails(QWidget *parent = nullptr, QStringList *l = nullptr);
    ~viewDetails();

private:
    Ui::viewDetails *ui;
};

#endif // VIEWDETAILS_H
