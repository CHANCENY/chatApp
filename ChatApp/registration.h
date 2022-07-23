#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include "profileupload.h"
#include <QDir>
#include "dbfunctions.h"
#include "mythread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Registration; }
QT_END_NAMESPACE

class Registration : public QDialog
{
    Q_OBJECT

private:
    QString fullName;
    QString phoneNumber;
    QString emailAddress;

public:
    Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:

    void on_phone_returnPressed();

    void on_email_returnPressed();

    void on_name_returnPressed();

private:
    Ui::Registration *ui;
    profileUpload *imagesupload;
};
#endif // REGISTRATION_H
