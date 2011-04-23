#include "messagehandler.h"
#include <iostream>
#include <QDateTime>
#include <QtCore>

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


//    if (messageParts.length()>1){
//        if (messageParts[1]=="LOGINWITHPASSWORD"){
//            if (messageParts.length()==3){
//                emit loginWithPassword(&(messageParts[0]),&(messageParts[2]));
//            }
//        }
//        if (messageParts[1]=="LOGINPHOTO"){
//			QByteArray image;
//			in >> image;
//			emit loginWithPicture(&(messageParts[0]),&image);
//        }
//        if (messageParts[1]=="LOGOUT"){
//            emit logout(&(messageParts[0]));
//        }
//        if (messageParts[1]=="GAMELIST"){
//            emit gamelist(&(messageParts[0]));
//        }
//        if (messageParts[1]=="CREATEGAME"){
//	    if (messageParts.length()==4){
//               emit createGame(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt());
//	    }
//        }
//        if (messageParts[1]=="SETSTARTTIME"){
//            if (messageParts.length()==3){
//                QDate temp;
//                emit setTime(&(messageParts[0]),&(temp.fromString(messageParts[2])));
//            }
//        }
//        if (messageParts[1]=="SETDURATION"){
//            if (messageParts.length()==3){
//                emit setDuration(&(messageParts[0]),messageParts[2].toInt());
//            }
//        }
//        if (messageParts[1]=="SETMAXPLAYERS"){
//            if (messageParts.length()==3){
//                emit setPlayers(&(messageParts[0]),messageParts[2].toInt());
//            }
//        }
//        if (messageParts[1]=="SETNUMBEROFTEAMS"){
//            if (messageParts.length()==3){
//                emit setTeams(&(messageParts[0]),messageParts[2].toInt());
//            }
//        }
//        if (messageParts[1]=="INVITE"){
//            if (messageParts.length()==4){
//                emit invite(&(messageParts[0]),&(messageParts[2]),messageParts[3].toInt());
//            }
//        }
//        if (messageParts[1]=="JOINGAME"){
//            if (messageParts.length()==3){
//                emit joinGame(&(messageParts[0]),&(messageParts[2]));
//            }
//        }
//        if (messageParts[1]=="JOINTEAM"){
//            if (messageParts.length()==3){
//                emit joinTeam(&(messageParts[0]),&(messageParts[2]));
//            }
//        }
//        if (messageParts[1]=="LEAVEGAME"){
//                emit leave(&(messageParts[0]));
//        }
//        if (messageParts[1]=="CANCELGAME"){
//                emit cancel(&(messageParts[0]));
//        }

//        if (messageParts[1]=="SHOOT"){
//			QByteArray image;
//			in >> image;
//			emit shoot(&(messageParts[0]), &image);
//        }
//        if (messageParts[1]=="GAMESTART"){
//                emit gameStart(&(messageParts[0]));
//        }

//    }

    if (messageParts[1] == "SHOOT") {
        QByteArray image;
        in >> image;
        QFile file("/home/pkliang/resized.jpg"); //write image to file image.jpg
        if (!file.open(QIODevice::WriteOnly))
            qDebug("can not save photo image");
        if(file.write(image) == -1)
            qDebug("saving image failed");
        file.close();
        //parsePicture(image);

        //TODO Send photo to game logic
       // sendMessage("lewang;ONTARGET;true;bob;");
        //sendMessage("lewang;ONTARGET;false;");
   }
    else if (messageParts[1] == "LOGINPHOTO") {
        QByteArray image;
        in >> image;
        //parsePicture(image);
        //TODO Send login photo to face recognition.
       // sendMessage("lewang;ONTARGET;true;bob;");
       // sendMessage("lewang;ONTARGET;false;");
   }
    else if (messageParts[1]=="LOGINPASSWD"){
        if (messageParts[2] == "aaa"){
            qDebug("passwd is aaa");
            sendMessage("lewang;LOGIN;true");
        }
        else
            sendMessage("lewang;LOGIN;false");
    }
    else if (messageParts[1]=="GAMELIST"){
         //fake game list
         QString gamelist("lewang;GAMELIST;game1;game2;game3;game4;");
         sendMessage(gamelist);
     }
    else if (messageParts[1]=="CREATEGAME"){
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
          sendMessage("lewang;GAMECREATED;true;");
        }
        else{
          sendMessage("lewang;GAMECREATED;false;");
        }
    }
    else if (messageParts[1] == "JOINGAME") {
         sendMessage("lewang;GAMEINFO;"+messageParts[2]+";100;4/5;3/5;");
    }
    else if (messageParts[1] == "LEAVEGAME") {

    }
    else if (messageParts[1] == "JOINTEAM") {
        sendMessage("lewang;TEAMJOINED;");
    }
    else if (messageParts[1] == "GAMESTART") {
         sendMessage("lewang;GAMESTARTED;");
    }
    else qDebug("unknown OPCODE");
//    qDebug() << "a user joins game";
//    sendMessage("lewang;USERJOIN;"+messageParts[2]+";100;4/5;3/5;Alice;");
//    qDebug() << "a user leaves game";
//    sendMessage("lewang;USERLEAVE;"+messageParts[2]+";100;4/5;3/5;Tom;");
//    //qDebug() << "game abort";
//    sendMessage("lewang;GAMEABORT;");
//    qDebug() << "game update";
//    sendMessage("lewang;100;4/5;4/5;Tim;Ali;70;true;");

    //QDateTime date;
    sendMessage(QString("received message"));
}
