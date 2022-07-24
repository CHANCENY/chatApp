#ifndef CHATROOM_H
#define CHATROOM_H

#include <QMainWindow>
#include <QStringList>
#include <QString>
#include "dbfunctions.h"
#include "mysocktes.h"
#include "options.h"
#include "addcontacts.h"
#include <QListWidgetItem>
#include <QListWidget>
#include "editcontact.h"
#include "contactactions.h"
#include <QSystemTrayIcon>
#include "passwordsettingandcode.h"

namespace Ui {
class ChatRoom;
}

class ChatRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatRoom(QWidget *parent = nullptr, QString profile=nullptr, QString phone =nullptr, QString fullname=nullptr);
    ~ChatRoom();




public slots:
    QString getAllMessages(QString number);
    void recentChecker();
    void upDateLocal();
    void getNewMessageFromServer();
    void sendingPendingMessages();
    void responsePendingSent();
    void updateContacts();
    void changeResponse();
    void restartCheckContacts();
    void validateSettings();
    void onActiveUpdate();
    void statuscheck();
    QString removeAllSpecial(QString line);
    QString removeAllSpecial2(QString line);





private slots:
    void on_recentchat_clicked(const QModelIndex &index);
    void displayMessages(QString messages);
    QListWidgetItem* getItems();

    void on_pushButton_clicked();

    void on_messagesToSend_textChanged();

    void on_messagedisplay_clicked(const QModelIndex &index);

    void on_messagedisplay_itemActivated(QListWidgetItem *item);


    void on_recentchat_itemDoubleClicked(QListWidgetItem *item);

    void on_optioning_clicked();

    void on_addcontact_clicked();

    void on_listcontacts_itemClicked(QListWidgetItem *item);

    void on_editicontact_clicked();

    void on_listcontacts_itemDoubleClicked(QListWidgetItem *item);

    void openChatWindow(QStringList number);
    bool addToRecentChat(QStringList li);
    void myProfileAccount();
    bool editAccountInfo(QString querys, QString querys2);
    void editAccountToServer(QString querys);

    void on_editname_returnPressed();

    void on_editemail_returnPressed();

    void on_nameeditbutton_clicked();

    void on_phoneeditbutton_clicked();

    void on_emaileditbutton_clicked();

    void on_editphone_returnPressed();

    void on_profileeditbutton_clicked();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_3_returnPressed();

    void on_deleteallbuttonmenu_clicked();

    void on_cleanllbuttonmenu_clicked();

    void on_loginenablemenu_stateChanged(int arg1);

    void popNotification();

    void on_steptwobuttonmenu_stateChanged(int arg1);



    void on_loginenablemenu_clicked();

    void on_steptwobuttonmenu_clicked();

    void on_IpAddressLineeditmenu_returnPressed();

    void on_checkBox_clicked();



private:
    Ui::ChatRoom *ui;
    mySocktes socket;
    Options *op;
    QListWidgetItem *items;
    AddContacts *contacts;
    editContact *edits;
    ContactActions *actions;
    QString myphoneNumber;
    QString myProfile;
    QSystemTrayIcon *pop;
    passwordSettingAndCode *settingupsecurity;

    void noteSettingChecker();
    bool checkingNotificationbox(QString note);

};

#endif // CHATROOM_H
