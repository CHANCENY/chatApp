#include "chatroom.h"
#include "ui_chatroom.h"
#include <QTimer>
#include <QMessageBox>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTextToSpeech>







DBFunctions rcheck;

QString numberfound = nullptr;
QString currentText = nullptr;

QTimer *pendingSender;
QTimer *recenttimer;
QTimer *updating;

QString ReplyMessages= nullptr;
QString contactSelected = nullptr;

QTimer *contactschecker;
QTimer *pops;
QTimer *settings;

QTimer *newonesfound;

QString messageArrived = nullptr, title = nullptr;
static int flagmade = 0;
static int clickednow = 0;
static int states = 0;

bool change = false;


QString querytochangenumber1, querytochangenumber2, newphonenumber;

QString noteSetting = nullptr;
QString voiceSettingnote = nullptr;



ChatRoom::ChatRoom(QWidget *parent, QString profile, QString phone, QString fullname) :
    QMainWindow(parent),
    ui(new Ui::ChatRoom)
{
    ui->setupUi(this);

    DBFunctions connection;
    connection.connectionSetUp();

    ui->replydisplay->setVisible(false);
    ui->messagesToSend->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);

    //for available people
    ui->addcontact->setVisible(false);
    ui->editicontact->setVisible(false);

    // account

    ui->editemail->setVisible(false);
    ui->editname->setVisible(false);
    ui->editphone->setVisible(false);


    if(profile != nullptr)
    {
        QPixmap pro(profile);
        ui->profile->setPixmap(pro);
        QIcon con(profile);
        ui->tabWidget->setTabIcon(2,con);
        this->myProfile = profile;
    }

    ui->name->setText(fullname);
    ui->phone->setText(phone);
    this->noteSettingChecker();


    this->myphoneNumber = phone;


    QStringList recent = rcheck.recentChat();

    ui->recentchat->clear();

    int sizes = recent.size();

    for(int i = 0; i < sizes; i++)
    {
        QStringList list = recent[i].split('@');
        if(list.last() == "New")
        {
           ui->recentchat->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-tick-box-48.png"),list[0]));

           continue;
        }

        ui->recentchat->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-old.png"),list[0]));

    }

    pendingSender = new QTimer(this);
    connect(pendingSender,SIGNAL(timeout()),this,SLOT(sendingPendingMessages()));
    pendingSender->start(7200);

    recenttimer = new QTimer(this);
    connect(recenttimer,SIGNAL(timeout()),this,SLOT(recentChecker()));
    recenttimer->start(5000);

     updating = new QTimer(this);
     connect(updating,SIGNAL(timeout()),this,SLOT(getNewMessageFromServer()));
     updating->start(2000);

     contactschecker = new QTimer(this);
     connect(contactschecker,SIGNAL(timeout()),this,SLOT(updateContacts()));
     contactschecker->start(2000);

      pops = new QTimer(this);
      connect(pops,SIGNAL(timeout()),this,SLOT(popNotification()));
      pops->start(1000);

      newonesfound = new QTimer(this);
      connect(newonesfound,SIGNAL(timeout()),this,SLOT(onActiveUpdate()));
      newonesfound->start(1000);

      QTimer *socketstatus = new QTimer(this);
      connect(socketstatus,SIGNAL(timeout()),this,SLOT(statuscheck()));
      socketstatus->start(1000);


      this->myProfileAccount();

}

ChatRoom::~ChatRoom()
{
    delete ui;
}

QString ChatRoom::getAllMessages(QString number)
{
    if(number != nullptr)
    {
        QString message = nullptr;

        QSqlQuery query;
        query.prepare("SELECT message FROM messages WHERE sendernumber= '"+number+"';");
        if(query.exec())
        {
            while(query.next())
            {
              message = query.value(0).toString();
            }
        }
        if(message != nullptr)
        {
            return message;
        }
    }
    return nullptr;
}

void ChatRoom::recentChecker()
{

    QStringList recent = rcheck.recentChat();   

     ui->recentchat->clear();

     int sizes = recent.size();

     for(int i = 0; i < sizes; i++)
     {
         QStringList list = recent[i].split('@');
         if(list.last() == "New")
         {
            ui->recentchat->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-tick-box-48.png"),list[0]));
            messageArrived ="New Message Arrived "+ list.first();
            title = "New Messages Notification!";
            continue;
         }

         ui->recentchat->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-old.png"),list[0]));
     }
}

void ChatRoom::upDateLocal()
{
   QDir dir;
   QString filename = dir.absoluteFilePath("DATA");

    QString messages = nullptr;
   QFile myfile(filename);
   myfile.open(QIODevice::ReadOnly);
   if(myfile.isOpen())
   {
       QTextStream in(&myfile);
       messages = in.readAll();
       myfile.close();
   }

   if(messages != nullptr && messages != "false")
   {
       QStringList list = messages.split('@');

       QString m , sn;
       for(int i = 0; i < list.size() - 1; i++)
       {
           QStringList mlist = list[i].split("#->");

           QSqlQuery query;
           query.prepare("SELECT * FROM messages WHERE sendernumber ='"+mlist[1]+"';");
           if(query.exec())
           {
               while(query.next())
               {
                     m = query.value(5).toString();
                     sn = query.value(0).toString();

               }
           }

           if(sn != nullptr)
           {
               QString allmes = m +"*#"+ mlist.last();
               query.prepare("UPDATE messages SET message = '"+allmes+"', date = '"+mlist[3]+"', status= '"+mlist[4]+"' WHERE sendernumber= '"+mlist[1]+"';");
               query.exec();
           }
           else if(sn == nullptr)
           {
               query.prepare("INSERT INTO messages VALUES('"+mlist[0]+"','"+mlist[1]+"','"+mlist[2]+"','"+mlist[3]+"','"+mlist[4]+"','"+mlist[5]+"');");
               query.exec();
           }
       }
   }
}

void ChatRoom::on_recentchat_clicked(const QModelIndex &index)
{
    currentText = ui->recentchat->model()->data(index).toString();

    if(currentText != nullptr)
    {

        numberfound = nullptr;

        QStringList numbers = rcheck.getNumbers();


        for(int i = 0; i < numbers.size(); i++)
        {
            if(currentText == rcheck.getName(numbers[i]))
            {
               numberfound = numbers[i];
            }
        }

        if(numberfound != nullptr)
        {
            this->displayMessages(rcheck.getMessagesBody(numberfound));
            rcheck.markOldMessage(numberfound);
            ui->conersation->setText("<font color='red'>Conversation with "+currentText+"</font>");
            ui->messagesToSend->setVisible(true);
            ui->pushButton->setVisible(true);
            ui->pushButton_2->setVisible(true);
        }
    }
}

void ChatRoom::displayMessages(QString messages)
{

  if(!messages.isEmpty() && messages.size() > 5 && messages != nullptr)
  {
      ui->messagedisplay->clear();

      QStringList mList = messages.split("*#");
      int sizes = mList.size();

      for(int i = 0; i < sizes - 1; i++)
      {
          QStringList lines = mList[i].split("->");
         // ui->messagedisplay->addItem(lines[0]);
          if(lines[1] == "rec")
          {
             ui->messagedisplay->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-inbox-32.png"),lines[0]+"\n"+this->removeAllSpecial2(lines.last())));
          }
          if(lines[1] == "sec")
          {
             ui->messagedisplay->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-sent-48.png"),lines[0]+"\n"+this->removeAllSpecial2(lines.last())));

          }
      }

      ui->messagedisplay->scrollToBottom();


  }
  else
  {
       ui->messagedisplay->clear();
  }
}

QListWidgetItem *ChatRoom::getItems()
{
  return this->items;
}

void ChatRoom::getNewMessageFromServer()
{
   QString line = "newmessages@"+ui->phone->text();
   QByteArray data = line.toLocal8Bit();
   socket.sendingMessages(data);
   QTimer::singleShot(1000,this,SLOT(upDateLocal()));
}

void ChatRoom::sendingPendingMessages()
{
   QSqlQuery query;
    QString allmessages = nullptr;
   query.prepare("SELECT * FROM pending WHERE status= 'New';");
   if(query.exec())
   {
       while(query.next())
       {
           QString line = query.value(0).toString();

           if(line != nullptr)
           {
               QStringList list = line.split("->");
               QString q = "pendingmessages@INSERT INTO messages VALUES('"+list[0]+"','"+list[1]+"','"+list[2]+"','"+list[3]+"','New','"+list[4]+"');%%%";
               allmessages = allmessages + q;
           }

       }
   }

   if(socket.sendingMessages(allmessages.toLocal8Bit()) == true)
   {

       updating->stop();
       recenttimer->stop();
       pendingSender->stop();
       QTimer::singleShot(2000,this,SLOT(responsePendingSent()));
   }

}

void ChatRoom::responsePendingSent()
{
    QDir dir;
    QString filename = dir.absoluteFilePath("DATA");

     QString messages = nullptr;
    QFile myfile(filename);
    myfile.open(QIODevice::ReadOnly);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
        messages = in.readAll();
        myfile.close();
    }

    if(messages == "true")
    {
        QSqlQuery query;
        query.prepare("DELETE FROM pending WHERE status = 'New';");
        if(query.exec())
        {
          pendingSender->stop();
          updating->start(2000);
          recenttimer->start(5000);
        }
    }
    else
    {
        updating->start(2000);
        recenttimer->start(5000);
        pendingSender->start(7200);
    }
}

void ChatRoom::updateContacts()
{
    QStringList contactsList;

    QSqlQuery query;
    query.prepare("SELECT * FROM contacts;");
    if(query.exec())
    {
        while(query.next())
        {
           contactsList.append(query.value(0).toString()+","+query.value(1).toString()+","+query.value(2).toString()+","+query.value(3).toString());
        }
    }

    if(contactsList.isEmpty())
    {
        return;
    }


    ui->listcontacts->clear();

    for(int i = 0; i < contactsList.size(); i++)
    {
        QStringList cList = contactsList[i].split(',');

        if(cList.last() != nullptr)
        {
           ui->listcontacts->addItem(new QListWidgetItem(QIcon(cList.last()),cList[0]+"\n"+cList[1]));
        }
        else
        {
            ui->listcontacts->addItem(new QListWidgetItem(QIcon(":/ic/icons/user.png"),cList[0]+"\n"+cList[1]));
        }

    }
}

void ChatRoom::changeResponse()
{
    QDir dir;
    QString filename = dir.absoluteFilePath("DATA");

     QString messages = nullptr;
    QFile myfile(filename);
    myfile.open(QIODevice::ReadOnly);
    if(myfile.isOpen())
    {
        QTextStream in(&myfile);
        messages = in.readAll();
        myfile.close();
    }

    if(messages == "true")
    {
        change = true;
        if(change == true)
        {
           change = this->editAccountInfo(querytochangenumber1,querytochangenumber2);
           if(change == true)
           {
               updating->start(2000);
               recenttimer->start(5000);
               pendingSender->start(7200);
               contactschecker->start(2000);
               ui->editphone->setVisible(false);
               ui->phone->setText(newphonenumber);
                ui->phonelabel->setText(newphonenumber);
               QMessageBox::information(0,"change committed","You have update your profile");
           }
        }
    }
    else
    {
        updating->start(2000);
        recenttimer->start(5000);
        pendingSender->start(7200);
        contactschecker->start(2000);
        QMessageBox::warning(0,"change committed","Network not available");
        change = false;
    }
}

void ChatRoom::restartCheckContacts()
{

  contactschecker->start(2000);

}

void ChatRoom::validateSettings()
{
   QSqlQuery query;
   query.prepare("SELECT * FROM owner WHERE phone = '"+ui->phone->text()+"';");
   if(query.exec())
   {
       while(query.next())
       {
           if(query.value(4).toString() != nullptr && flagmade == 1)
           {
               title = "Login Security Notification";
               messageArrived = "You have enabled login security protocal with password "+query.value(4).toString();
               settings->stop();
               ui->loginenablemenu->setChecked(true);
           }
           if(query.value(5).toString() != nullptr && flagmade == 2)
           {
               title = "Step two verification Notification";
               messageArrived = "You have enabled Step two verification security protocal with code "+query.value(6).toString();
               settings->stop();
               ui->steptwobuttonmenu->setChecked(true);
           }

       }
   }
}

void ChatRoom::onActiveUpdate()
{
   DBFunctions newones;
   QString bodyMessages = newones.onActiveUpdatingChat(numberfound);
   if(bodyMessages != nullptr)
   {
      this->displayMessages(this->removeAllSpecial2(bodyMessages));
       messageArrived ="New Message Arrived "+ currentText;
       title = "New Messages Notification!";
       ui->messagedisplay->scrollToBottom();
       bodyMessages = nullptr;

   }
}

void ChatRoom::statuscheck()
{
   QByteArray result = socket.getMessage();
   std::string n = result.toStdString();
   QPixmap status(QString::fromStdString(n));
   ui->status->setPixmap(status);
}

QString ChatRoom::removeAllSpecial(QString line)
{
  if(line != nullptr)
  {
      for(int i = 0; i < line.length(); i++)
      {
          if(line[i] == '@')
          {
              line[i] = '^';
          }
      }
      return line;
  }
  return nullptr;
}

QString ChatRoom::removeAllSpecial2(QString line)
{
    if(line != nullptr)
    {
        for(int i = 0; i < line.length(); i++)
        {
            if(line[i] == '^')
            {
                line[i] = '@';
            }
        }
        return line;
    }
    return nullptr;
}

void ChatRoom::popNotification()
{
  if(messageArrived != nullptr && noteSetting =="On")
  {
      pop = new QSystemTrayIcon(this);
      pop->setIcon(QIcon(":/ic/icons/chat.png"));
      pop->setVisible(true);
      pop->showMessage(title,messageArrived,QIcon(":/ic/icons/chat.png"),10000);
      messageArrived = nullptr;

  }

  if(messageArrived != nullptr && voiceSettingnote == "On")
  {
     QTextToSpeech *speaker = new QTextToSpeech;
     speaker->setVolume(100);
     speaker->say(messageArrived);
     if(speaker->state() != 1)
     {
         messageArrived = nullptr;
     }
  }
}

void ChatRoom::on_pushButton_clicked()
{
   QString messages =this->removeAllSpecial(ui->messagesToSend->toPlainText());
   QString tobeSendMessage = messages;
   messages = messages.trimmed();

   if(messages != nullptr && currentText != nullptr && numberfound != nullptr)
   {
       QDate date = QDate::currentDate();
       QString dates = date.toString("dd-MM-yy");

       QString myNumber = ui->phone->text();
       QString myName =ui->name->text();

       if(ReplyMessages != nullptr)
       {
           tobeSendMessage = nullptr;
           tobeSendMessage = "reply: "+ReplyMessages+"\n ans: "+messages;
       }
       QString line = "sending@INSERT INTO messages VALUES('"+numberfound+"','"+myNumber+"','"+myName+"','"+dates+"','New','"+tobeSendMessage+"');";

        /*updating->stop();
        recenttimer->stop();
        pendingSender->stop();*/
       bool s = socket.sendingMessages(line.toLocal8Bit());
       if( s == false)
       {
           QString pend = numberfound+"->"+myNumber+"->"+myName+"->"+dates+"->"+tobeSendMessage;
           rcheck.pendingToSend(pend);

           QString body = dates+"->sec->"+this->removeAllSpecial2(tobeSendMessage);

           QStringList mmm; mmm.append(numberfound); mmm.append(myNumber); mmm.append(myName);
           mmm.append(dates); mmm.append("Old"); mmm.append(body);

           if(rcheck.saveMessagesLocal(mmm))
           {               
               ui->messagedisplay->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-sent-48.png"),dates+"\n"+this->removeAllSpecial2(tobeSendMessage)));
               ui->messagedisplay->scrollToBottom();
               ui->messagesToSend->clear();
               ui->replydisplay->clear();
               ui->replydisplay->setVisible(false);
               ReplyMessages = nullptr;
               updating->start(2000);
               recenttimer->start(5000);
               pendingSender->start(7200);
               contactschecker->start(2000);

           }
       }
       else if(s == true)
       {
           QString body = dates+"->sec->"+this->removeAllSpecial2(tobeSendMessage);

           QStringList mmm; mmm.append(numberfound); mmm.append(myNumber); mmm.append(myName);
           mmm.append(dates); mmm.append("Old"); mmm.append(body);

           if(rcheck.saveMessagesLocal(mmm))
           {
              ui->messagedisplay->addItem(new QListWidgetItem(QIcon(":/ic/icons/icons8-sent-48.png"),dates+"\n"+this->removeAllSpecial2(tobeSendMessage)));
              ui->messagedisplay->scrollToBottom();
              ui->messagesToSend->clear();
              ui->replydisplay->clear();
              ui->replydisplay->setVisible(false);
              ReplyMessages = nullptr;
              updating->start(2000);
              recenttimer->start(5000);
              pendingSender->start(7200);
              contactschecker->start(2000);

           }

       }


   }
   else
   {
       ui->messagesToSend->clear();
   }
}

void ChatRoom::on_messagesToSend_textChanged()
{
   QString text = ui->messagesToSend->toPlainText();
   if(text != nullptr)
   {
       updating->stop();
       recenttimer->stop();
       pendingSender->stop();
       contactschecker->stop();

       for(int i = 0; i < text.length(); i++)
       {
           if(text[i] == '^')
           {
               text[i] = ' ';
               ui->messagesToSend->clear();
               ui->messagesToSend->insertPlainText(text);
           }
       }
   }
   else
   {
       updating->start(2000);
       recenttimer->start(5000);
       pendingSender->start(7200);
       contactschecker->start(2000);
   }
}

void ChatRoom::on_messagedisplay_clicked(const QModelIndex &index)
{
     QString currentText = ui->messagedisplay->model()->data(index).toString();

     if(currentText!= nullptr)
     {
         int position = ui->messagedisplay->currentRow();
         int last = ui->messagedisplay->count();

         op = new Options(this,currentText, last,position,numberfound);
         op->show();

     }
}

void ChatRoom::on_messagedisplay_itemActivated(QListWidgetItem *item)
{
  QString activated = ui->messagedisplay->currentItem()->text();
  qDebug()<<item;

  QStringList list = activated.split("\n");
  ui->replydisplay->setVisible(true);
  ReplyMessages = list.last();
  ui->replydisplay->setText("<font color= 'green'>Reply: "+list.last()+"</font>");


}

void ChatRoom::on_recentchat_itemDoubleClicked(QListWidgetItem *item)
{
    QString sendername = ui->recentchat->currentItem()->text();
   // QString cov = ui->conersation->text();


    int ok = QMessageBox::question(0,"Deleting","Do you want to delete this person from recent chat ?","Yes","No","Cancel",0,-1);
    if(ok == 0)
    {
        int flag = 0;
        QSqlQuery query;
        query.prepare("DELETE FROM messages WHERE sendername = '"+sendername+"';");
        if(query.exec())
        {
           flag = 1;
        }

        if(flag == 1)
        {
          ui->messagedisplay->clear();
          ui->conersation->clear();
        }

    }
    Q_UNUSED(item);

}

void ChatRoom::on_optioning_clicked()
{
    if(ui->addcontact->isVisible() && ui->editicontact->isVisible())
    {
        ui->addcontact->setVisible(false);
        ui->editicontact->setVisible(false);

    }
    else
    {
        ui->addcontact->setVisible(true);
        ui->editicontact->setVisible(true);

    }
}

void ChatRoom::on_addcontact_clicked()
{
    contacts = new AddContacts(this,pendingSender,recenttimer,updating,contactschecker);
    contacts->show();
}

void ChatRoom::on_listcontacts_itemClicked(QListWidgetItem *item)
{
   //contactschecker->stop();
   contactSelected = ui->listcontacts->currentItem()->text();

   QStringList list = contactSelected.split("\n");

   actions = new ContactActions(this,list.last(),list.first(),contactschecker);
   actions->show();

   Q_UNUSED(item);

}

void ChatRoom::on_editicontact_clicked()
{
    edits = new editContact(this,pendingSender,recenttimer,updating,contactschecker);
    edits->show();
}

void ChatRoom::on_listcontacts_itemDoubleClicked(QListWidgetItem *item)
{
   QStringList List = ui->listcontacts->currentItem()->text().split("\n");

   if(List.isEmpty())
   {
       return;
   }

   this->openChatWindow(List);
   Q_UNUSED(item);

}

void ChatRoom::openChatWindow(QStringList number)
{

    if(!number.isEmpty())
    {
      if(this->addToRecentChat(number))
      {
          ui->tabWidget->setCurrentIndex(0);
          this->displayMessages(rcheck.getMessagesBody(number.last()));
          ui->messagesToSend->setVisible(true);
          ui->pushButton->setVisible(true);
          numberfound = number.last();
          currentText = number.first();
          ui->conersation->setText("<font color='red'>Conversation with "+number.first()+"</font>");
          QMessageBox::information(0,"Notification","You can start chatting with "+number.first());
      }
    }


}

bool ChatRoom::addToRecentChat(QStringList li)
{
    QString myName = ui->name->text();
    QString myNumber = ui->phone->text();



    if(li.isEmpty())
    {
        return false;
    }

    if(myName == nullptr && myNumber == nullptr)
    {
        return false;
    }

    QString db = nullptr;

    QSqlQuery query; query.prepare("SELECT * FROM messages WHERE sendernumber = '"+li.last()+"' AND sendername ='"+li.first()+"';");
    if(query.exec())
    {
        while(query.next())
        {
           db = query.value(0).toString();
        }
    }

    if(db == nullptr)
    {
        query.prepare("INSERT INTO messages(reciever,sendernumber,sendername,status) VALUES('"+myNumber+"','"+li.last()+"','"+li.first()+"','Old');");
        if(query.exec())
        {
            return true;
        }
    }

    if(db != nullptr)
    {
        return true;
    }

    return false;
}

void ChatRoom::myProfileAccount()
{
  QSqlQuery query;
  query.prepare("SELECT * FROM owner WHERE phone ='"+ui->phone->text()+"';");
  if(query.exec())
  {
      while(query.next())
      {
          ui->namelabel->setText(query.value(0).toString());
          ui->phonelabel->setText(query.value(1).toString());
          ui->emaillabel->setText(query.value(2).toString());

          if(query.value(3).toString() != nullptr)
          {
              QPixmap pro(query.value(3).toString());
              ui->profileimages->setPixmap(pro);
          }

          if(query.value(4).toString() != nullptr)
          {

              ui->loginenablemenu->setChecked(true);
              //settings->stop();

          }
          if(query.value(5).toString() == "true")
          {

              ui->steptwobuttonmenu->setChecked(true);
              //settings->stop();

          }
      }
  }
}

bool ChatRoom::editAccountInfo(QString querys, QString querys2)
{
  int flag = 0;
  QSqlQuery query;
  query.prepare(querys);
  if(query.exec())
  {
     flag = 2;
  }

  if(flag == 2)
  {
      query.prepare(querys2);
      if(query.exec())
      {
          return true;
      }
  }

  return false;
}

void ChatRoom::editAccountToServer(QString querys)
{
    updating->stop();
    recenttimer->stop();
    pendingSender->stop();
    contactschecker->stop();
    QString line = "changeAccountInfo@"+querys;
    socket.sendingMessages(line.toLocal8Bit());
    QTimer::singleShot(1000,this,SLOT(changeResponse()));
}

void ChatRoom::on_editname_returnPressed()
{
    QString newname = ui->editname->text();

    if(newname !=  nullptr)
    {
        QString line = "UPDATE owner SET fullname = '"+newname+"' WHERE phone = '"+this->myphoneNumber+"';";
        QSqlQuery query; query.prepare(line);
        if(query.exec())
        {
            ui->editname->setText(nullptr);
            ui->editname->setVisible(false);
            ui->name->setText(newname);
            ui->namelabel->setText(newname);
        }
    }
}


void ChatRoom::on_editemail_returnPressed()
{
    QString newemail = ui->editemail->text();

    if(newemail !=  nullptr)
    {
        QString line = "UPDATE owner SET email = '"+newemail+"' WHERE phone = '"+this->myphoneNumber+"';";
        QSqlQuery query; query.prepare(line);
        if(query.exec())
        {
            ui->editemail->setText(nullptr);
            ui->editemail->setVisible(false);
             ui->emaillabel->setText(newemail);

        }
    }
}


void ChatRoom::on_nameeditbutton_clicked()
{
    if(ui->editname->isVisible())
    {
        ui->editname->setVisible(false);
    }
    else
    {
         ui->editname->setVisible(true);
    }
}


void ChatRoom::on_phoneeditbutton_clicked()
{
    if(ui->editphone->isVisible())
    {
        ui->editphone->setVisible(false);
    }
    else
    {
         ui->editphone->setVisible(true);
    }
}


void ChatRoom::on_emaileditbutton_clicked()
{
    if(ui->editemail->isVisible())
    {
        ui->editemail->setVisible(false);
    }
    else
    {
         ui->editemail->setVisible(true);
    }
}


void ChatRoom::on_editphone_returnPressed()
{
    QString newnumber = ui->editphone->text();

    if(newnumber != nullptr && newnumber.size() == 10)
    {
        QString line2 = "UPDATE messages SET sendernumber = '"+newnumber+"' WHERE sendernumber = '"+ui->phone->text()+"';";
        QString line = "UPDATE owner SET phone = '"+newnumber+"' WHERE phone = '"+ui->phone->text()+"';";
        querytochangenumber1 = line; querytochangenumber2 = line2;
        newphonenumber = newnumber;

        this->editAccountToServer(line2);

    }
}


void ChatRoom::on_profileeditbutton_clicked()
{
    QString newfile = QFileDialog::getOpenFileName(0,"New image",QDir::homePath(),"*.jpg; *.jpeg; *.png");
    if(newfile != nullptr)
    {
        QFileInfo info(newfile);

        if(info.size() < 2000000)
        {
            QStringList list = {"jpg","jpeg","png"};
            QString extension = info.fileName().split('.').last();

            if(contacts->in_array(extension,list))
            {
                QString target = "PicturesUploaded/"+info.fileName();
                QByteArray data;

                QFile myfile(newfile);
                myfile.open(QIODevice::ReadOnly);
                if(myfile.isOpen())
                {
                   data = myfile.readAll();
                   myfile.close();
                }

                if(data.isEmpty())
                {
                    QMessageBox::warning(0,"warning","Failed to extract data of "+info.fileName()+" to upload");
                    return;
                }

                if(contacts->move_file_upload(data,target))
                {
                    QPixmap pro(target);
                    ui->profileimages->setPixmap(pro);
                    ui->profile->setPixmap(pro);

                    ui->tabWidget->setTabIcon(2,QIcon(target));
                }
            }


        }
        else
        {
            QMessageBox::warning(0,"Size warning","This file size is ("+QString::number(+info.size() / 1000000)+"MB) You file is too big (2MB) max is allowed!");
        }
    }
}


void ChatRoom::on_lineEdit_3_textChanged(const QString &arg1)
{

   // searching function

    contactschecker->stop();
    ui->listcontacts->clear();

    if(arg1 != nullptr)
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM contacts WHERE fullname ='"+arg1+"' OR phone ='"+arg1+"' OR email = '"+arg1+"';");
        if(query.exec())
        {
            while(query.next())
            {
                if(query.value(3).toString() != nullptr)
                {
                   ui->listcontacts->addItem(new QListWidgetItem(QIcon(query.value(3).toString()),query.value(0).toString()+"\n"+query.value(1).toString()));
                }
                else
                {
                    ui->listcontacts->addItem(new QListWidgetItem(QIcon(":/ic/icons/user.png"),query.value(0).toString()+"\n"+query.value(1).toString()));
                }
            }
        }
    }

    QTimer::singleShot(5000,this,SLOT(restartCheckContacts()));
}


void ChatRoom::on_lineEdit_3_returnPressed()
{
    // searching function
    QString arg1 = ui->lineEdit_3->text();
     contactschecker->stop();
     ui->listcontacts->clear();

     if(arg1 != nullptr)
     {
         QSqlQuery query;
         query.prepare("SELECT * FROM contacts WHERE fullname ='"+arg1+"' OR phone ='"+arg1+"' OR email = '"+arg1+"';");
         if(query.exec())
         {
             while(query.next())
             {
                 if(query.value(3).toString() != nullptr)
                 {
                    ui->listcontacts->addItem(new QListWidgetItem(QIcon(query.value(3).toString()),query.value(0).toString()+"\n"+query.value(1).toString()));
                 }
                 else
                 {
                     ui->listcontacts->addItem(new QListWidgetItem(QIcon(":/ic/icons/user.png"),query.value(0).toString()+"\n"+query.value(1).toString()));
                 }
             }
         }
     }

     QTimer::singleShot(5000,this,SLOT(restartCheckContacts()));
}


void ChatRoom::on_deleteallbuttonmenu_clicked()
{
        QStringList images;
        QSqlQuery query;
        query.prepare("SELECT photo FROM contacts;");
        if(query.exec())
        {
            while(query.next())
            {
               images.append(query.value(0).toString());
            }
        }

        images.append(this->myProfile);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("DATABASE.db");
        db.open();
        if(db.isOpen())
        {
           db.close();
           db = QSqlDatabase();
           QSqlDatabase::removeDatabase("DATABASE.db");
           if(db.connectionName().isEmpty())
           {
               QFile::remove("DATABASE.db");
               QFile::remove("storage/DATABASE.db");
               QFile::remove("storage/ipAdress.txt");
               QFile::remove("DATA");

               for(int i = 0; i < images.size(); i++)
               {
                   QFile::remove(images[i]);
               }
               QDir dir;
               dir.rmdir("PicturesUploaded");
               dir.rmdir("storage");
               this->close();
           }

        }
        else
        {
          qDebug()<<"is not open "<<db;
        }


}


void ChatRoom::on_cleanllbuttonmenu_clicked()
{
    QString line ="DELETE FROM messages;";
    DBFunctions dele;

    if(dele.deleteAccount(line))
    {
        QMessageBox::information(0,"Delete notification","All chat have been deleted successfully");
    }
}


void ChatRoom::on_loginenablemenu_stateChanged(int arg1)
{
    if(arg1 == 2 && clickednow == 1)
    {
        QString myNumber = ui->phone->text();
        settingupsecurity = new passwordSettingAndCode(0,myNumber,1);
        settingupsecurity->show();
        flagmade = 1;
        settings = new QTimer(this);
        connect(settings,SIGNAL(timeout()),this,SLOT(validateSettings()));
        settings->start(3000);
    }
    else if(arg1 == 0)
    {
        QSqlQuery query;
        query.prepare("UPDATE owner SET password = '' WHERE phone ='"+ui->phone->text()+"';");
        if(query.exec())
        {
            title = "Login Security protocal";
            messageArrived ="You have removed login security protocal";
        }
    }
}


void ChatRoom::on_steptwobuttonmenu_stateChanged(int arg1)
{
    if(arg1 == 2 && clickednow == 2)
    {
        QString myNumber = ui->phone->text();
        settingupsecurity = new passwordSettingAndCode(0,myNumber,2);
        settingupsecurity->show();
        flagmade = 2;
        settings = new QTimer(this);
        connect(settings,SIGNAL(timeout()),this,SLOT(validateSettings()));
        settings->start(3000);
    }
    else if(arg1 == 0)
    {
        QSqlQuery query;
        query.prepare("UPDATE owner SET steptwo = '', stepcode = '' WHERE phone ='"+ui->phone->text()+"';");
        if(query.exec())
        {
            title = "Step two verification Security protocal";
            messageArrived ="You have removed Step two verification security protocal";
        }
    }
}


void ChatRoom::on_loginenablemenu_clicked()
{
    clickednow = 1;
}


void ChatRoom::on_steptwobuttonmenu_clicked()
{
    clickednow = 2;
}


void ChatRoom::on_IpAddressLineeditmenu_returnPressed()
{
   if(ui->IpAddressLineeditmenu->text() != nullptr)
   {
       DBFunctions ipc;
       if(ipc.addAddressNetwork(QStringList()<<ui->IpAddressLineeditmenu->text()))
       {
           title = "Network Notification";
           messageArrived ="You have Set up new ip address of "+ui->IpAddressLineeditmenu->text();
       }
   }
}


void ChatRoom::noteSettingChecker()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM notification;");
    if(query.exec())
    {
        while(query.next())
        {
            noteSetting = query.value(0).toString();
            voiceSettingnote = query.value(1).toString();
        }
    }

    if(noteSetting == "On")
    {
        ui->checkBox->setChecked(true);
        ui->checkBox->setCheckable(true);
    }

    if(voiceSettingnote == "On")
    {
        ui->checkBox_2->setChecked(true);
        ui->checkBox_2->setCheckable(true);
    }
}


bool ChatRoom::checkingNotificationbox(QString note)
{
    QSqlQuery query;
    QString old = nullptr;
    query.prepare("SELECT * FROM notification;");
    if(query.exec())
    {
        while(query.next())
        {
            old = query.value(0).toString();

        }
    }

    if(old != nullptr)
    {
        query.prepare("UPDATE notification SET status = '"+note+"' WHERE status = '"+old+"';");
        if(query.exec())
        {
            return true;
        }
    }
    else
    {
        query.prepare("INSERT INTO notification VALUES('"+note+"');");
        if(query.exec())
        {
            return true;
        }
    }
    return false;
}


void ChatRoom::on_checkBox_clicked(bool checked)
{
    if(checked == true)
    {
        QSqlQuery query;
        query.prepare("UPDATE notification SET status = 'On' WHERE status = 'Off';");
        if(query.exec())
        {
            noteSetting = "On";
        }
    }
    else
    {
        QSqlQuery query;
        query.prepare("UPDATE notification SET status = 'Off' WHERE status = 'On';");
        if(query.exec())
        {
            noteSetting = "Off";
        }
    }
}


void ChatRoom::on_checkBox_2_clicked(bool checked)
{
    if(checked == true)
    {
        QSqlQuery query;
        query.prepare("UPDATE notification SET voice = 'On' WHERE voice = 'Off';");
        if(query.exec())
        {
            voiceSettingnote = "On";
        }
    }
    else
    {
        QSqlQuery query;
        query.prepare("UPDATE notification SET voice = 'Off' WHERE voice = 'On';");
        if(query.exec())
        {
            voiceSettingnote = "Off";
        }
    }
}


void ChatRoom::on_pushButton_2_clicked()
{
    QString tosend = ui->messagesToSend->toPlainText();
    if(tosend != nullptr)
    {
        QTextToSpeech * s = new QTextToSpeech;
        s->setVolume(100);
        s->say(tosend);
    }
}

