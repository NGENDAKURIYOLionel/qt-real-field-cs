#include <QApplication>
#include <QtCore>
#include <QDebug>

#include <stdlib.h>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    Server server;
    QApplication app(argc, argv);

    if (server.listen(QHostAddress::Any, 8888)) {
        qDebug() << "port:" << server.serverPort() <<"\n";
    }
    else {
        qDebug() << "server port is unkown\n";
    }

    return app.exec();
}
