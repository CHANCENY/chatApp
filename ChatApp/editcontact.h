#ifndef EDITCONTACT_H
#define EDITCONTACT_H

#include <QDialog>
#include <QtSql>
#include <QTimer>
#include "dbfunctions.h"
#include <QFile>
#include <QDir>

namespace Ui {
class editContact;
}

class editContact : public QDialog
{
    Q_OBJECT

public:
    explicit editContact(QWidget *parent = nullptr, QTimer *t1 = nullptr, QTimer *t2 = nullptr, QTimer *t3 = nullptr, QTimer *t4 = nullptr);
    ~editContact();
    void fetchContact(QString line);
    bool uniqueChecker(QString values, QString type);
    bool processingImage(QString file);

private slots:
    void on_uploadp_clicked();

    void on_fullname_textChanged(const QString &arg1);

    void on_phonenumber_textChanged(const QString &arg1);

    void on_email_textChanged(const QString &arg1);

    void on_addcontact_clicked();

private:
    Ui::editContact *ui;
};

#endif // EDITCONTACT_H
