#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "dbfunctions.h"
#include <QTimer>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr,QString current = nullptr, int last = 0, int row = 0, QString number = nullptr);
    ~Options();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Options *ui;
    QString currentText;
    int lastIndex;
    int row;
    QString numberPhone;

};

#endif // OPTIONS_H
