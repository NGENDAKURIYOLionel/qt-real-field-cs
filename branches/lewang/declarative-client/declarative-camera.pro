include(../../staticconfig.pri)
include (../examples.pri)

TEMPLATE=app

QT += declarative network

!maemo* {
    contains(QT_CONFIG, opengl) {
        QT += opengl
    }
}

SOURCES += $$PWD/qmlcamera.cpp \
    client.cpp \
    cli.cpp
!mac:TARGET = qml_camera
else:TARGET = QmlCamera

RESOURCES += declarative-camera.qrc

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

HEADERS += \
    client.h \
    cli.h
