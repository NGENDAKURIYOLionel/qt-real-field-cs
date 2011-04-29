#include "server.h"
#include <QtNetwork/QTcpServer>
#include <playerconnectionthread.h>

Server::Server(QObject *parent) : QTcpServer(parent)
{
	extern DataBaseHelper* db_global;
    this->db = db_global;
}

void Server::incomingConnection(int socketDescriptor)
{
    PlayerConnectionThread *thread = new PlayerConnectionThread(socketDescriptor, "test message",this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
