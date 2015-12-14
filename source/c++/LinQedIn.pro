#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T16:38:59
#
#-------------------------------------------------
QT       += xml
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LinQedIn
TEMPLATE = app
DESTDIR = $$PWD

SOURCES += main.cpp\
    utente.cpp \
    username.cpp \
    profilo.cpp \
    info.cpp \
    utentebasic.cpp \
    utentebusiness.cpp \
    utenteexecutive.cpp \
    smartutente.cpp \
    database.cpp \
    admin.cpp \
    client.cpp \
    rete.cpp \
    widget.cpp \
    clickablelabel.cpp

HEADERS  += \
    utente.h \
    username.h \
    profilo.h \
    info.h \
    utentebasic.h \
    utentebusiness.h \
    utenteexecutive.h \
    smartutente.h \
    database.h \
    admin.h \
    client.h \
    rete.h \
    widget.h \
    eccezioni.h \
    clickablelabel.h
