#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T21:44:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MinimapView
TEMPLATE = app

LIBS += ../stormlib/libstorm.a

SOURCES += main.cpp\
        mainwindow.cpp \
    mapview.cpp

HEADERS  += mainwindow.h \
    mapview.h

FORMS    += mainwindow.ui
