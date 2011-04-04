#include "messagehandler.h"
#include <iostream>
#include <QDateTime>

MessageHandler::MessageHandler(QTcpSocket *socket)
{
    tcpsocket=socket;
}

void MessageHandler::sendMessage(QString message){
    tcpsocket->write(message.toAscii());
}


void MessageHandler::readMessage(){
    QTextStream in(tcpsocket);
    std::cout << in.readAll().toStdString() <<"\n";
    QDateTime date;
    sendMessage(QString("received message").append(date.toString()));
}
