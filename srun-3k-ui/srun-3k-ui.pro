#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T19:20:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = srun-3k-ui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS\
       CURL_STATICLIB\
       DBUILDING_LIBCURL\
       DCURL_STATICLIB


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS    +=  -LC:\Users\Administrator.PC-201611240416\Desktop\curl-7.54.0\lib -static-libgcc -lcurl -lws2_32 -lwldap32

INCLUDEPATH += C:\Users\Administrator.PC-201611240416\Desktop\curl-7.54.0\include

QMAKE_CFLAGS += -std=c99

CONFIG += c++11

RC_FILE += main.rc
