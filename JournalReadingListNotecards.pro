QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 testcase no_testcase_installs

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dailynotecardsdialog.cpp \
    datepicker.cpp \
    filterorrows.cpp \
    journal.cpp \
    journalencrypter.cpp \
    journalsqlmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    mouseclickfilter.cpp \
    notecardinputdialog.cpp \
    notecards.cpp \
    notecardsdelegate.cpp \
    notecardssqlmodel.cpp \
    readinglist.cpp \
    readinglistdelegate.cpp \
    readinglistsqlmodel.cpp \
    settingsdialog.cpp

HEADERS += \
    dailynotecardsdialog.h \
    datepicker.h \
    filterorrows.h \
    journal.h \
    journalencrypter.h \
    journalsqlmodel.h \
    mainwindow.h \
    mouseclickfilter.h \
    notecardinputdialog.h \
    notecards.h \
    notecardsdelegate.h \
    notecardssqlmodel.h \
    readinglist.h \
    readinglistdelegate.h \
    readinglistsqlmodel.h \
    settingsdialog.h

FORMS += \
    dailynotecardsdialog.ui \
    datepicker.ui \
    journal.ui \
    mainwindow.ui \
    notecardinputdialog.ui \
    notecards.ui \
    readinglist.ui \
    settingsdialog.ui


LIBS += -LC:/Qt/Tools/OpenSSL/Win_x64/lib libcrypto.lib libssl.lib
INCLUDEPATH += "C:/Qt/Tools/OpenSSL/Win_x64/include"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
