#ifndef PROFILEUPLOAD_H
#define PROFILEUPLOAD_H

#include <QDialog>
#include <chatroom.h>
#include <QString>
#include <QFileDialog>
#include "dbfunctions.h"

namespace Ui {
class profileUpload;
}

class profileUpload : public QDialog
{
    Q_OBJECT

private:
    QString fullName;
    QString phoneNumber;
    QString emailAddress;
    int checkedAgreement;
    QByteArray imageProfile;



public:
    explicit profileUpload(QWidget *parent = nullptr);
   explicit profileUpload(QWidget *parent = nullptr, QString *fullName= nullptr, QString *phoneNumber = nullptr, QString *emailAddress = nullptr);
    ~profileUpload();

    bool in_Array(QString hint, QStringList stacks);
    bool move_file_upload(QByteArray filedata, QString destination);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::profileUpload *ui;
    ChatRoom *room;
};

#endif // PROFILEUPLOAD_H
