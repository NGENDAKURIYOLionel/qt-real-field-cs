#ifndef PLAYERCONNECTIONTHREAD_H
#define PLAYERCONNECTIONTHREAD_H

#include <qthread.h>
#include <QtNetwork/qtcpsocket.h>
#include "messagehandler.h"

class PlayerConnectionThread :public QThread
{
    Q_OBJECT

public:
    PlayerConnectionThread(int socketDescriptor,const QString &text, QObject *parent);
    void run();

signals:
     void error(QTcpSocket::SocketError socketError);     

private:
    int socketDescriptor;
    QString message;



};

#endif // PLAYERCONNECTIONTHREAD_H
