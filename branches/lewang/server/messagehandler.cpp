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

    qDebug() << "---Debugging messages from cpp---";
    qDebug() << message;
    foreach(QString msg, messageParts)
        qDebug() << msg;
    qDebug() << "---Debugging messages from cpp---";

    if (messageParts[1]=="LOGINPASSWD"){
        if (messageParts[2] == "aaa"){
            sendMessage("lewang;LOGIN;true");
        }
        else
            sendMessage("lewang;LOGIN;false");
    }
    if (messageParts[1]=="GAMELIST"){
         //fake game list
         QString gamelist("lewang;game1;game2;game3;game4;");
         sendMessage(gamelist);
     }
    if (messageParts[1]=="CREATEGAME"){
        QString error = "";
        if (messageParts[2].size() > 6)
            error = error + "GameID is no more than 6 characters\n";
        int time = messageParts[3].toInt();
        if (time <= 60 || time >= 6000)
            error = error + "Time is no less than 60 seconds and larger than 6000 seconds\n";
        int noTeamA =  messageParts[4].toInt();
        if (noTeamA <= 0 || noTeamA > 20)
            error = error + "Number of team A is no less than 0 and more than 20\n";
        int noTeamB = messageParts[5].toInt();
        if (noTeamB <= 0 || noTeamB > 20)
            error = error + "Number of team B is no less than 0 and more than 20\n";

        // success or fail should be decided by server side as well
        if (error.isEmpty()) {
          sendMessage("lewang;GAMECTEATED;true;");
        }
        else{
          sendMessage("lewang;GAMECTEATED;false;");
        }
    }
    if (messageParts[1] == "JOINGAME") {
         sendMessage("lewang;GAMEINFO;"+messageParts[2]+";100;4/5;3/5;");
    }
    if (messageParts[1] == "LEAVEGAME") {

    }
    if (messageParts[1] == "JOINTEAM") {
        sendMessage("lewang;TEAMJOINED;");
    }
    if (messageParts[1] == "GAMESTART") {
         sendMessage("lewang;GAMESTARTED;");
    }
    if (messageParts[1] == "SHOOT") {
        sendMessage("lewang;ONTARGET;true;bob;");
        sendMessage("lewang;ONTARGET;false;");
   }

//    qDebug() << "a user joins game";
//    sendMessage("lewang;USERJOIN;"+messageParts[2]+";100;4/5;3/5;Alice;");
//    qDebug() << "a user leaves game";
//    sendMessage("lewang;USERLEAVE;"+messageParts[2]+";100;4/5;3/5;Tom;");
//    //qDebug() << "game abort";
//    sendMessage("lewang;GAMEABORT;");
//    qDebug() << "game update";
//    sendMessage("lewang;100;4/5;4/5;Tim;Ali;70;true;");

}
