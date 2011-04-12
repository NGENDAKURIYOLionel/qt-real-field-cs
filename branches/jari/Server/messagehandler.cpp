#include "messagehandler.h"
#include <iostream>
#include <QDateTime>
#include <QtCore>
#include <QImage>

MessageHandler::MessageHandler(QTcpSocket *socket)
{
    tcpsocket=socket;
}

void MessageHandler::sendMessage(QString message){
    tcpsocket->write(message.toAscii());
}

void MessageHandler::sendMessageSlot(QString message){
    sendMessage(message);
}

QImage MessageHandler::parsePicture(QString message){
    //should parse the picture message here
    QImage temp;
    return temp;
}


void MessageHandler::readMessage(){
    QTextStream in(tcpsocket);
    QString message = in.readAll();
    QStringList messageParts;
    messageParts=message.split(";");

    if (messageParts.length()>1){
        if (messageParts[1]=="LOGINWITHPASSWORD"){
            if (messageParts.length()==3){
                emit loginWithPassword(messageParts[0],messageParts[2]);
            }
        }
        if (messageParts[1]=="LOGINWITHPICTURE"){
            if (messageParts.length()==3){
                emit loginWithPicture(messageParts[0],parsePicture(messageParts[2]));
            }
        }
        if (messageParts[1]=="LOGOUT"){
            emit logout(messageParts[0]);
        }
        if (messageParts[1]=="GAMELIST"){
            emit gamelist(messageParts[0]);
        }
        if (messageParts[1]=="CREATEGAME"){
            emit createGame(messageParts[0]);
        }
        if (messageParts[1]=="SETSTARTTIME"){
            if (messageParts.length()==3){
                QDate temp;
                emit setTime(messageParts[0],temp.fromString(messageParts[2]));
            }
        }
        if (messageParts[1]=="SETDURATION"){
            if (messageParts.length()==3){
                emit setDuration(messageParts[0],messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="SETMAXPLAYERS"){
            if (messageParts.length()==3){
                emit setPlayers(messageParts[0],messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="SETNUMBEROFTEAMS"){
            if (messageParts.length()==3){
                emit setTeams(messageParts[0],messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="INVITE"){
            if (messageParts.length()==4){
                emit invite(messageParts[0],messageParts[2],messageParts[3].toInt());
            }
        }
        if (messageParts[1]=="JOINGAME"){
            if (messageParts.length()==3){
                emit joinGame(messageParts[0],messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="JOINTEAM"){
            if (messageParts.length()==3){
                emit joinTeam(messageParts[0],messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="LEAVEGAME"){
                emit leave(messageParts[0]);
        }
        if (messageParts[1]=="CANCELGAME"){
                emit cancel(messageParts[0]);
        }

        if (messageParts[1]=="SHOOT"){
            if (messageParts.length()==3){
                emit shoot(messageParts[0],parsePicture(messageParts[2]));
            }
        }
        if (messageParts[1]=="GAMESTART"){
                emit gameStart(messageParts[0]);
        }

    }
    //QDateTime date;
    //sendMessage(QString("received message").append(date.toString()));
}
