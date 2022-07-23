#ifndef CONTACTACTIONS_H
#define CONTACTACTIONS_H

#include <QDialog>
#include <QtSql>
#include "dbfunctions.h"
#include "viewdetails.h"

namespace Ui {
class ContactActions;
}

class ContactActions : public QDialog
{
    Q_OBJECT

public:
    explicit ContactActions(QWidget *parent = nullptr,QString number = nullptr, QString name = nullptr, QTimer *t = nullptr);
    ~ContactActions();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ContactActions *ui;
    QString name;
    QString number;
    viewDetails *views;
};

#endif // CONTACTACTIONS_H
