#-------------------------------------------------
#
# Project created by QtCreator 2013-05-10T18:10:38
#
#-------------------------------------------------

QT       += core gui sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Security-Sys-gui
TEMPLATE = app
INCLUDEPATH += "/home/tomer/bbboo/include"
LIBS += -L"/home/tomer/bbboo/lib" -lboost_system -lboost_thread -lpthread -lboost_serialization


SOURCES += main.cpp\
        mainwindow.cpp \
    pictureview.cpp \
    Server.cpp \
    ClientConnection.cpp \
    AsyncSerializationConnection.cpp \


HEADERS  += mainwindow.h \
    pictureview.h \
    Server.h \
    ClientConnection.h \
    PacketForServer.h \
    PacketForClient.h


FORMS    += mainwindow.ui \
    pictureview.ui
