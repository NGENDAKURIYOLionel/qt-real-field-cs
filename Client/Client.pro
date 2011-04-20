#-------------------------------------------------
#
# Project created by QtCreator 2011-03-21T07:27:26
#
#-------------------------------------------------

QT       += core
QT       += network


TARGET = Client
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += mobility
MOBILITY = bearer

TEMPLATE = app


SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h
