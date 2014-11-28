#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T21:44:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MinimapView
TEMPLATE = app

LIBS += ../casclib/libcasc.a

SOURCES += main.cpp\
        mainwindow.cpp \
    mapview.cpp \
    maploader.cpp \
    cascstorage.cpp \
    cascfile.cpp \
    dbcreader.cpp \
    abstractdbcformat.cpp \
    dbcmap.cpp

HEADERS  += mainwindow.h \
    mapview.h \
    maploader.h \
    cascstorage.h \
    cascstorage_p.h \
    cascfile.h \
    dbcreader.h \
    abstractdbcformat.h \
    dbcreader_p.h \
    dbcmap.h

FORMS    += mainwindow.ui
