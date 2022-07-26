QT       += core gui sql network texttospeech
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcontacts.cpp \
    chatroom.cpp \
    contactactions.cpp \
    dbfunctions.cpp \
    editcontact.cpp \
    forgotpassword.cpp \
    loginpage.cpp \
    main.cpp \
    mydatabase.cpp \
    mysocktes.cpp \
    mythread.cpp \
    networkdilog.cpp \
    options.cpp \
    passwordsettingandcode.cpp \
    profileupload.cpp \
    registration.cpp \
    steptwo.cpp \
    viewdetails.cpp

HEADERS += \
    addcontacts.h \
    chatroom.h \
    contactactions.h \
    dbfunctions.h \
    editcontact.h \
    forgotpassword.h \
    loginpage.h \
    mydatabase.h \
    mysocktes.h \
    mythread.h \
    networkdilog.h \
    options.h \
    passwordsettingandcode.h \
    profileupload.h \
    registration.h \
    steptwo.h \
    viewdetails.h

FORMS += \
    addcontacts.ui \
    chatroom.ui \
    contactactions.ui \
    editcontact.ui \
    forgotpassword.ui \
    loginpage.ui \
    networkdilog.ui \
    options.ui \
    passwordsettingandcode.ui \
    profileupload.ui \
    registration.ui \
    steptwo.ui \
    viewdetails.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
    RC_ICONS = chat.ico
