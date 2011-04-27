#include "playerconnectionthread.h"
#include <QtNetwork>
#include <iostream>

PlayerConnectionThread::PlayerConnectionThread(int socketDescriptor, const QString &text, Server *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), message(text)
{

}

void PlayerConnectionThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    MessageHandler handler(&tcpSocket);
    connect(&tcpSocket,SIGNAL(readyRead()),&handler,SLOT(readMessage()));

    //tcpSocket.write(message.toAscii());
    handler.sendMessage(message);

    tcpSocket.waitForDisconnected(600000);
    //std::cout << "disconnected" ;
}
