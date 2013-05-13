#-------------------------------------------------
#
# Project created by QtCreator 2013-05-10T18:10:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Security-Sys-gui
TEMPLATE = app

LIBS += -L/usr/lib -lboost_system -lboost_thread -lpthread -lboost_serialization


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../workspace/Server/src/Server.cpp \
    ../../workspace/Server/src/ClientConnection.cpp \
    ../../workspace/Server/src/AsyncSerializationConnection.cpp \
    qtserver.cpp \
    ../../FinalProject/FtpClient/FTPListParse.cpp \
    ../../FinalProject/FtpClient/FTPFileStatus.cpp \
    ../../FinalProject/FtpClient/FTPDataTypes.cpp \
    ../../FinalProject/FtpClient/FTPClient.cpp \
    ../../FinalProject/FtpClient/BlockingSocket.cpp \
    pictureview.cpp

HEADERS  += mainwindow.h \
    ../../workspace/Server/src/Server.h \
    ../../workspace/Server/src/Packet.h \
    ../../workspace/Server/src/ClientConnection.h \
    qtserver.h \
    ../../FinalProject/FtpClient/smart_ptr.h \
    ../../FinalProject/FtpClient/FTPListParse.h \
    ../../FinalProject/FtpClient/FTPFileStatus.h \
    ../../FinalProject/FtpClient/FTPDataTypes.h \
    ../../FinalProject/FtpClient/FTPClient.h \
    ../../FinalProject/FtpClient/Definements.h \
    ../../FinalProject/FtpClient/BlockingSocket.h \
    pictureview.h

FORMS    += mainwindow.ui \
    pictureview.ui
