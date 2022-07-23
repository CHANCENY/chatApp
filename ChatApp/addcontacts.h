#ifndef ADDCONTACTS_H
#define ADDCONTACTS_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include "dbfunctions.h"

namespace Ui {
class AddContacts;
}

class AddContacts : public QDialog
{
    Q_OBJECT

public:
    explicit AddContacts(QWidget *parent = nullptr, QTimer *t1 = nullptr, QTimer *t2 = nullptr, QTimer *t3 = nullptr, QTimer *t4 = nullptr);
    ~AddContacts();
    bool in_array(QString hint, QStringList list);
    bool move_file_upload(QByteArray filename, QString destination);
    bool uniqueCotactCheck(QString values, QString type);

private slots:


    void on_uploadp_clicked();

    void on_addcontact_clicked();

    void on_email_textChanged(const QString &arg1);

    void on_fullname_textChanged(const QString &arg1);

    void on_phonenumber_textChanged(const QString &arg1);

private:
    Ui::AddContacts *ui;
};

#endif // ADDCONTACTS_H
