#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T14:18:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qurtain
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newrestdialog.cpp

HEADERS  += mainwindow.h \
    rest.h \
    singleshotrest.h \
    intervalrest.h \
    newrestdialog.h

FORMS    += mainwindow.ui \
    newrestdialog.ui
