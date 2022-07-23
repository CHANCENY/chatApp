#ifndef NETWORKDILOG_H
#define NETWORKDILOG_H

#include <QDialog>
#include <QThread>
#include "registration.h"

namespace Ui {
class networkDilog;
}

class networkDilog : public QDialog
{
    Q_OBJECT

public:
    explicit networkDilog(QWidget *parent = nullptr, QThread *thread = nullptr);
    ~networkDilog();

public slots:
    void changing();

private:
    Ui::networkDilog *ui;
    QThread *threadin;
    Registration *regirs;
};

#endif // NETWORKDILOG_H
