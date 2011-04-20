#-------------------------------------------------
#
# Project created by QtCreator 2011-03-21T05:29:46
#
#-------------------------------------------------

QT       += core
QT       += network

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    server.cpp \
    playerconnectionthread.cpp \
    messagehandler.cpp

HEADERS += \
    server.h \
    playerconnectionthread.h \
    messagehandler.h
