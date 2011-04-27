#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include "DataBaseHelper.h"
#include "playerfactory.h"
#include "gamefactory.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    DataBaseHelper *db;


protected:
    void incomingConnection(int socketDescriptor);

};

#endif // SERVER_H
