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
    qDebug( "---Debugging messages from cpp--- ");
    qDebug("sending message");
//    if(tcpsocket->write(message.toAscii()) == -1)
//        qDebug("send message failed");
      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out.setVersion(QDataStream::Qt_4_7);
      out << quint16(0) << message;
      out.device()->seek(0);
      out << quint16(block.size() - sizeof(quint16));
      tcpsocket->write(block);
    qDebug("done");
    qDebug( "---Debugging messages from cpp--- ");
}

void MessageHandler::sendMessageSlot(QString message){
    sendMessage(message);
}

void MessageHandler::parsePicture(QByteArray image){
    //should parse the picture message here
    //QByteArray image = message.toLatin1(); //image
    qDebug("parsePicture");
    QFile file("/home/pkliang/image.jpg"); //write image to file image.jpg
    if (!file.open(QIODevice::WriteOnly))
        qDebug("can not save photo image");
    if(file.write(image) == -1)
        qDebug("saving image failed");
    file.close();
}


void MessageHandler::readMessage(){
//      QTextStream in(tcpsocket);
//      QString message = in.readAll();
//      qDebug() << "bytesavailable is: " <<  tcpsocket->bytesAvailable() ;
//      if (tcpsocket->bytesAvailable() < 10000)
//      return;
//        QByteArray buf = tcpsocket->readAll();
//        qDebug() << QString(buf);

//      QFile file("/home/pkliang/resized.jpg");
//      file.open(QIODevice::ReadOnly);
//      QByteArray pic;
//      pic = file.readAll();
//      file.close();
//      QByteArray block;
//      QDataStream out(&block, QIODevice::WriteOnly);
//      out << quint16(0) << QString("test;LOGINPHOTO;") << pic;
//      out.device()->seek(0);
//      out << quint16(block.size() - sizeof(quint16));

//      QDataStream in(&block, QIODevice::ReadOnly);
//      quint16 size;
//      QString opcode;
//      QByteArray image;
//      in >> size >> opcode >> image;
//      qDebug() << size << opcode;
//      //out << pic;
//      QFile write("/home/pkliang/test.jpg");
//      write.open(QIODevice::WriteOnly);
//      write.write(image);
//      write.close();

    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_4_7);
    if (nextBlockSize == 0 ) {
        if(tcpsocket->bytesAvailable() < sizeof(quint16))
            return;
        in >> nextBlockSize;
    }
    qDebug() << "nextBlocksize is: " << nextBlockSize;
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

    if (messageParts[1] == "SHOOT") {
        QByteArray image;
        in >> image;
        parsePicture(image);

        //TODO Send photo to game logic
       // sendMessage("lewang;ONTARGET;true;bob;");
        //sendMessage("lewang;ONTARGET;false;");
   }
    else if (messageParts[1] == "LOGINPHOTO") {
        QByteArray image;
        in >> image;
        parsePicture(image);
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

}
