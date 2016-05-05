#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T19:49:58
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    msglist.cpp

HEADERS  += mainwindow.h \
    msglist.h

FORMS    += mainwindow.ui \
    msglist.ui
