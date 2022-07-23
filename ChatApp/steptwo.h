#ifndef STEPTWO_H
#define STEPTWO_H

#include <QDialog>
#include "dbfunctions.h"
#include "chatroom.h"

namespace Ui {
class StepTwo;
}

class StepTwo : public QDialog
{
    Q_OBJECT

public:
    explicit StepTwo(QWidget *parent = nullptr, QString *phone = nullptr);
    ~StepTwo();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::StepTwo *ui;
    const QString *phone;
    ChatRoom *room;

    bool stepTwoVerification(QString code);
};

#endif // STEPTWO_H
