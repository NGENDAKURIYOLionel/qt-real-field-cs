#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QtCore>
#include <QImage>

class MessageHandler : public QObject
{
     Q_OBJECT

public:
    MessageHandler(QTcpSocket *socket);
    void sendMessage(QString message);

public slots:
    void readMessage();
    void sendMessageSlot(QString message);

signals:
    void loginWithPassword(QString uname,QString password);
    void loginWithPicture(QString uname,QImage picture);
    void logout(QString uname);
    void gamelist(QString uname);
    void createGame(QString uname, QString gameId, int duration);
    void setTime(QString uname,QDate date);
    void setDuration(QString uname,int duration);
    void setPlayers(QString uname,int players);
    void setTeams(QString uname,int teams);
    void invite(QString uname,QString targetName,int gameId);
    void joinGame(QString uname,QString gameId);
    void joinTeam(QString uname,QString teamId);
    void leave(QString uname);
    void cancel(QString uname);
    void shoot(QString uname,QImage picture);
    void gameStart(QString uname);

private:
    QTcpSocket *tcpsocket;
    QImage parsePicture(QString message);
};

#endif // MESSAGEHANDLER_H
