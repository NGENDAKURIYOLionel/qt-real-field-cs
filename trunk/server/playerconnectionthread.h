#ifndef PLAYERCONNECTIONTHREAD_H
#define PLAYERCONNECTIONTHREAD_H

#include <qthread.h>
#include <QtNetwork/qtcpsocket.h>
#include "messagehandler.h"
#include "server.h"

class PlayerConnectionThread :public QThread
{
    Q_OBJECT

public:
    PlayerConnectionThread(int socketDescriptor,const QString &text, Server *parent);
    void run();


signals:
     void error(QTcpSocket::SocketError socketError);     

private:
    int socketDescriptor;
    QString message;

};

#endif // PLAYERCONNECTIONTHREAD_H
