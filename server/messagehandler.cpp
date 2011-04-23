#include "messagehandler.h"
#include <iostream>
#include <QDateTime>
#include <QtCore>
#include <QImage>

MessageHandler::MessageHandler(QTcpSocket *socket)
{
    tcpsocket=socket;
    nextBlockSize=0;
}

void MessageHandler::sendMessage(QString message){
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(0) << message;
    qDebug() << out;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
    qDebug() << out;
    qDebug() << "message: " << message;
    qDebug() << "size: " << block.size() - sizeof(quint32);
    tcpsocket->write(block);

    //tcpsocket->write(message.toAscii());
}

void MessageHandler::sendMessageSlot(QString message){
    sendMessage(message);
}

QImage* MessageHandler::parsePicture(QByteArray image){
    //should parse the picture message here
    qDebug("parsePicture");
    QFile file("/home/pkliang/image.jpg"); //write image to file image.jpg
    if (!file.open(QIODevice::WriteOnly))
        qDebug("can not save photo image");
    if(file.write(image) == -1)
        qDebug("saving image failed");
    file.close();
       QImage temp;
       return &temp;
}


void MessageHandler::readMessage(){
//    QTextStream in(tcpsocket);
//    QString message = in.readAll();
//    QStringList messageParts;
//    messageParts=message.split(";");
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_7);
    if (nextBlockSize == 0 ) {
        if(tcpsocket->bytesAvailable() < sizeof(quint32))
            return;
        in >> nextBlockSize;
    }
    qDebug() << "nextBlocksize is: " << nextBlockSize;
    qDebug() << "bytesAvailable is "<< tcpsocket->bytesAvailable();
    if (tcpsocket->bytesAvailable() < nextBlockSize)
        return;
    nextBlockSize = 0;
    QStringList messageParts;
    QString message;
    qDebug() << "bytesAvailable is "<< tcpsocket->bytesAvailable();
    in >> message;

    qDebug() << message;
    //sendMessage("test;LOGINPASSWD;aaa");
    messageParts = message.split(";");


    if (messageParts.length()>1){
        if (messageParts[1]=="LOGINWITHPASSWORD"){
            if (messageParts.length()==3){
                emit loginWithPassword(&(messageParts[0]),&(messageParts[2]));
            }
        }
        if (messageParts[1]=="LOGINPHOTO"){
            //if (messageParts.length()==3){
               // emit loginWithPicture(&(messageParts[0]),parsePicture(&(messageParts[2])));
                QByteArray image;
                in >> image;
                parsePicture(image);
           // }
        }
        if (messageParts[1]=="LOGOUT"){
            emit logout(&(messageParts[0]));
        }
        if (messageParts[1]=="GAMELIST"){
            emit gamelist(&(messageParts[0]));
        }
        if (messageParts[1]=="CREATEGAME"){
	    if (messageParts.length()==4){
               emit createGame(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt());
	    }
        }
        if (messageParts[1]=="SETSTARTTIME"){
            if (messageParts.length()==3){
                QDate temp;
                emit setTime(&(messageParts[0]),&(temp.fromString(messageParts[2])));
            }
        }
        if (messageParts[1]=="SETDURATION"){
            if (messageParts.length()==3){
                emit setDuration(&(messageParts[0]),messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="SETMAXPLAYERS"){
            if (messageParts.length()==3){
                emit setPlayers(&(messageParts[0]),messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="SETNUMBEROFTEAMS"){
            if (messageParts.length()==3){
                emit setTeams(&(messageParts[0]),messageParts[2].toInt());
            }
        }
        if (messageParts[1]=="INVITE"){
            if (messageParts.length()==4){
                emit invite(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt());
            }
        }
        if (messageParts[1]=="JOINGAME"){
            if (messageParts.length()==3){
                emit joinGame(&(messageParts[0]),&(messageParts[2]));
            }
        }
        if (messageParts[1]=="JOINTEAM"){
            if (messageParts.length()==3){
                emit joinTeam(&(messageParts[0]),&(messageParts[2]));
            }
        }
        if (messageParts[1]=="LEAVEGAME"){
                emit leave(&(messageParts[0]));
        }
        if (messageParts[1]=="CANCELGAME"){
                emit cancel(&(messageParts[0]));
        }

        if (messageParts[1]=="SHOOT"){
            if (messageParts.length()==3){

                //emit shoot(&(messageParts[0]),parsePicture(&(messageParts[2])));
            }
        }
        if (messageParts[1]=="GAMESTART"){
                emit gameStart(&(messageParts[0]));
        }

    }
    //QDateTime date;
    sendMessage(QString("received message"));
}
