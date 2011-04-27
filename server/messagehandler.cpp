#include "messagehandler.h"
#include <iostream>
#include <QDateTime>
#include <QtCore>
#include "player.h"
#include "playerfactory.h"
MessageHandler::MessageHandler(QTcpSocket *socket)
{
    tcpsocket=socket;
    nextBlockSize=0;
}

void MessageHandler::sendMessage(QString message){
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    //out.setVersion(QDataStream::Qt_4_7);
    out << quint32(0) << message;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
//    qDebug() << "message: " << message;
//    qDebug() << "size: " << block.size() - sizeof(quint32);
    tcpsocket->write(block);

}

void MessageHandler::sendMessageSlot(QString message){
    sendMessage(message);
}

void MessageHandler::loggedOut(){
    return;
}

void MessageHandler::readMessage(){
    QDataStream in(tcpsocket);
    //in.setVersion(QDataStream::Qt_4_7);
    if (nextBlockSize == 0 ) {
        if(tcpsocket->bytesAvailable() < sizeof(quint32))
            return;
        in >> nextBlockSize;
    }
//    qDebug() << "nextBlocksize is: " << nextBlockSize;
//    qDebug() << "bytesAvailable is "<< tcpsocket->bytesAvailable();
    if (tcpsocket->bytesAvailable() < nextBlockSize)
        return;
    nextBlockSize = 0;
    QStringList messageParts;
    QString message;
//    qDebug() << "bytesAvailable is "<< tcpsocket->bytesAvailable();
    in >> message;

    messageParts = message.split(";");

    // messageParts[0] is always the USERNAME
    if (messageParts.length()>1){
        Player* user=PlayerFactory::getPlayer(&messageParts[0]);
        if (user!=NULL){
            //message handler to player
            connect(this,SIGNAL(loginWithPassword(QString*,QString*)),user,SLOT(loginWithPassword(QString*,QString*)));
            connect(this, SIGNAL(loginWithPicture(QString*,QByteArray*)), user, SLOT(loginWithPicture(QString*,QByteArray*)));
            connect(this,SIGNAL(logout(QString*)),user,SLOT(logout(QString*)));
            connect(this,SIGNAL(createGame(QString*,QString*,int)),user,SLOT(createGame(QString*,QString*,int)));
            connect(this,SIGNAL(joinGame(QString*,QString*)),user,SLOT(joinGame(QString*,QString*)));
            connect(this,SIGNAL(joinTeam(QString*,QString*)),user,SLOT(joinTeam(QString*,QString*)));
            connect(this,SIGNAL(leave(QString*)),user,SLOT(leave(QString*)));
            connect(this,SIGNAL(shoot(QString*,QByteArray*)),user,SLOT(shoot(QString*,QByteArray*)));
            connect(this,SIGNAL(gameStart(QString*)),user,SLOT(gameStart(QString*)));
            //player to message handler
            connect(user,SIGNAL(loggedInSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(loggedOutSignal()),this,SLOT(loggedOut()));
            connect(user,SIGNAL(gameCreatedSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(gameInfoSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(joinedSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(gameStartedSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(gameAbortedSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(gameEndedSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(hitSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(updateSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(updatePlayerStatusSignal(QString)),this,SLOT(sendMessageSlot(QString)));
            connect(user,SIGNAL(playerInvitedSignal(QString)),this,SLOT(sendMessageSlot(QString)));

            if (messageParts[1] == "SHOOT") {
                QByteArray image;
                in >> image;
                emit shoot(&(messageParts[0]), &image);
    //            // This is how to save received file to image
    //            QFile file("~/resized.jpg"); //write image to file image.jpg
    //            if (!file.open(QIODevice::WriteOnly))
    //                qDebug("can not save photo image");
    //            if(file.write(image) == -1)
    //                qDebug("saving image failed");
    //            file.close();

            }
            if (messageParts[1] == "LOGINPHOTO") {
                QByteArray image;
                in >> image;
                emit loginWithPicture(&(messageParts[0]),&image);
            }
            if (messageParts[1]=="LOGINPASSWD"){
                if (messageParts.length()==3)
                   emit loginWithPassword(&(messageParts[0]),&(messageParts[2]));
            }
            if (messageParts[1]=="GAMELIST"){
                QList<QString*> games=GameFactory::getGameIds();
                QString message("GAMELIST;");
                for(int i;i<games.size();i++){
                    message.append(games.at(i));
                }
                sendMessage(message);
                //emit gamelist(&(messageParts[0]));
            }
            if (messageParts[1]=="LOGOUT"){
                emit logout(&(messageParts[0]));
            }
            if (messageParts[1]=="CREATEGAME"){
                if (messageParts.length()==6){
                       //emit createGame(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt(), messageParts[4].toInt(), messageParts[5].toInt());
                       emit createGame(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt());
                }
            }
            if (messageParts[1] == "JOINGAME") {
                if (messageParts.length()==3){
                    emit joinGame(&(messageParts[0]),&(messageParts[2]));
                    }
            }
            if (messageParts[1] == "LEAVEGAME") {
                emit leave(&(messageParts[0]));
            }
            if (messageParts[1] == "JOINTEAM") {
                if (messageParts.length()==3){
                    emit joinTeam(&(messageParts[0]),&(messageParts[2]));
                }
            }
            if (messageParts[1] == "GAMESTART") {
                emit gameStart(&(messageParts[0]));
            }
        else qDebug("unknown OPCODE");
        }
    }
}
