#include <QApplication>
#include <QtCore>

#include <stdlib.h>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    Server server;
    QApplication app(argc, argv);
//    Dialog dialog(server);
//    dialog.show();
//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    return dialog.exec();
    if (server.listen()) {
        std::cout << "port:" << server.serverPort() <<"\n";
    }
    else {
        std::cout << "server port is unkown\n";
    }

    return app.exec();
}
