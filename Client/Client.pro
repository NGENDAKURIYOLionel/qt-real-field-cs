#-------------------------------------------------
#
# Project created by QtCreator 2011-03-21T07:27:26
#
#-------------------------------------------------


TEMPLATE=app

QT += core declarative network
CONFIG += qdbus \
          mobility \
          console


!maemo* {
    contains(QT_CONFIG, opengl) {
        QT += opengl
    }
}

SOURCES += $$PWD/main.cpp \
    client.cpp

HEADERS += \
    client.h

!mac:TARGET = client
else:TARGET = client

RESOURCES += client.qrc

symbian {
    load(data_caging_paths)
    TARGET.CAPABILITY = UserEnvironment NetworkServices Location ReadUserData WriteUserData
    TARGET.EPOCHEAPSIZE = 0x20000 0x3000000
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}


