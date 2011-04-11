#include <QApplication>
#include <QtGui>
#include "client.h"


Client::Client () {
    connect(this, SIGNAL(clicked()),
            this, SLOT(send()));
}

void Client::sendMessage(){
    //tcpSocket->write(message.toAscii());
     qDebug() << "TEST";
     emit loginFailed();
}


void Client::send(){
    qDebug() << "TEST";
}
