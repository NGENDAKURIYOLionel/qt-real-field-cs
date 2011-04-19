#-------------------------------------------------
#
# Project created by QtCreator 2011-03-08T21:25:25
#
#-------------------------------------------------

QT       += core
QT       += network

TARGET = server
CONFIG   += console
#CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
	imagerecognitionhelper.cpp \
	json/json_value.cpp \
	json/json_reader.cpp \
	json/json_writer.cpp \
	server.cpp \
	playerconnectionthread.cpp \
	messagehandler.cpp \
	main.cpp


HEADERS += \
	imagerecognitionhelper.h \
	server.h \
	playerconnectionthread.h \
	messagehandler.h


#win32:LIBS += -L../server/curl -lcurldll
win32 {
	LIBS += -L$$PWD\curl -lcurldll
}
unix {
	LIBS += -lcurl # dunno which lib to link against on linux
}
