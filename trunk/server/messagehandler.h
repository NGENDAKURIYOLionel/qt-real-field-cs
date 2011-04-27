#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QtCore>
#include "server.h"


class MessageHandler : public QObject
{
     Q_OBJECT

public:
    MessageHandler(QTcpSocket *socket);
    void sendMessage(QString message);

public slots:
    void readMessage();
    void sendMessageSlot(QString message);
    void loggedOut();

signals:
    //Caution:
    //Pass pointers to slot functions may crash the program if slot functions uses these pointers after returning back
    //to the functions which emit these singals
    void loginWithPassword(QString* uname,QString* password);
    void loginWithPicture(QString* uname,QByteArray* picture);
    void logout(QString* uname);
    void gamelist(QString* uname);
    //void createGame(QString* uname, QString* gameId, int duration, int noOfTeamA, int noOfTeamB);
    void createGame(QString* uname, QString* gameId, int duration);
    void joinGame(QString* uname,QString* gameId);
    void joinTeam(QString* uname,QString* teamId);
    void leave(QString* uname);
    void shoot(QString* uname,QByteArray* picture);
    void gameStart(QString* uname);

      //uncessary signals according to ../message/message.png
//    void setTime(QString* uname,QDate* date);
//    void setDuration(QString* uname,int duration);
//    void setPlayers(QString* uname,int players);
//    void setTeams(QString* uname,int teams);
//    void invite(QString* uname,QString* targetName,int gameId);
//    void cancel(QString* uname);

private:
    QTcpSocket *tcpsocket;
    quint32 nextBlockSize;
};

#endif // MESSAGEHANDLER_H
