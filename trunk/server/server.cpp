#include "server.h"
#include <QtNetwork/QTcpServer>
#include <playerconnectionthread.h>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    db=new DataBaseHelper();
}

void Server::incomingConnection(int socketDescriptor)
{
    PlayerConnectionThread *thread = new PlayerConnectionThread(socketDescriptor, "test message",this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
