#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include <QDialog>
#include <QSystemTrayIcon>

namespace Ui {
class forgotpassword;
}

class forgotpassword : public QDialog
{
    Q_OBJECT

public:
    explicit forgotpassword(QWidget *parent = nullptr, QString phone = nullptr);
    ~forgotpassword();
    void sendCode();
    bool checkDigits(QString digits);
    bool change(QString nepassword);
    void messagePopUp(QString message, QString title);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::forgotpassword *ui;
    QString myPhoneNumber;
};

#endif // FORGOTPASSWORD_H
