#-------------------------------------------------
#
# Project created by QtCreator 2011-05-08T17:32:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cmdirh
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
	test_irh.cpp \
	imagerecognitionhelper.cpp \
    json/json_value.cpp \
    json/json_reader.cpp \
    json/json_writer.cpp

HEADERS += \
	imagerecognitionhelper.h

win32 {
	LIBS += -L$$PWD\curl -lcurldll
}
unix {
	LIBS += -lcurl
}
