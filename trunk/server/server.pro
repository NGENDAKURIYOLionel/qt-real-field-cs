#-------------------------------------------------
#
# Project created by QtCreator 2011-03-08T21:25:25
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
	imagerecognitionhelper.cpp

HEADERS += \
	imagerecognitionhelper.h

#win32:LIBS += -L../server/curl -lcurldll
win32 {
	LIBS += -L$$PWD\curl -lcurldll
}
unix {
	LIBS += -lcurl # dunno which lib to link against on linux
}
