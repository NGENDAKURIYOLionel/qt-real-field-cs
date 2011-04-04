#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QTcpSocket>

class MessageHandler : public QObject
{
     Q_OBJECT

public:
    MessageHandler(QTcpSocket *socket);
    void sendMessage(QString message);

public slots:
    void readMessage();

private:
    QTcpSocket *tcpsocket;
};

#endif // MESSAGEHANDLER_H
