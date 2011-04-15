#include <QApplication>
#include <QtGui>
#include "cli.h"


Cli::Cli () {
    connect(this, SIGNAL(clicked()),
            this, SLOT(send()));
}

void Cli::sendMessage(){
    //tcpSocket->write(message.toAscii());
     qDebug() << "TEST";
     emit loginFailed();
}


void Cli::send(){
    qDebug() << "TEST";
}
