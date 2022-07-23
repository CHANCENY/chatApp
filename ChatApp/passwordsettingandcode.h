#ifndef PASSWORDSETTINGANDCODE_H
#define PASSWORDSETTINGANDCODE_H

#include <QDialog>
#include <QtSql>
#include <QString>

namespace Ui {
class passwordSettingAndCode;
}

class passwordSettingAndCode : public QDialog
{
    Q_OBJECT

public:
    explicit passwordSettingAndCode(QWidget *parent = nullptr, QString number = nullptr, int type = 0);
    ~passwordSettingAndCode();

private slots:
    void on_password_clicked();

private:
    Ui::passwordSettingAndCode *ui;
    QString myNumber;
    int TypeCondition;
};

#endif // PASSWORDSETTINGANDCODE_H
