#-------------------------------------------------
#
# Project created by QtCreator 2016-08-06T13:36:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CuterTables
TEMPLATE = app

CONFIG+=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    cutertablemodel.cpp

HEADERS  += mainwindow.h \
    cutertablemodel.h

FORMS    += mainwindow.ui

DISTFILES += \
    books.json
