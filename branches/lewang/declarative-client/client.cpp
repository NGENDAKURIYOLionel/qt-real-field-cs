#include <QtCore>
#include <QImage>
#include <QDBusConnection>
#include <QDBusMessage>
#include "client.h"


Client::Client()
: _userName(""),
  _gameId("")
{

}

void Client::sendImage(const QByteArray &image){
    QByteArray buf1 = image.left(image.indexOf(";"));//userName
    QByteArray buf2 = image.right(image.size() - image.indexOf(";") - 1);
    QByteArray buf3 = buf2.left(buf2.indexOf(";")); //LOGINPHOTO
    QByteArray buf4 = buf2.right(buf2.size() - buf2.indexOf(";") - 1); //Image

    //qDebug() << buf1 << buf3;

    if(QString(buf3) == "LOGINPHOTO") {
        QFile file("/home/user/MyDocs/DCIM/test.jpg");
        if (!file.open(QIODevice::WriteOnly))
            qDebug("can not save photo image");
        file.write(buf4);
        file.close();
        emit loginSuccess();
    }
    else {
     // should be replaced by message sent from server to see if it is correct login or not
        emit loginFailed();
    }

    if(QString(buf3) == "SHOOT") {
        QFile file("/home/user/MyDocs/DCIM/shoot.jpg");
        if (!file.open(QIODevice::WriteOnly))
            qDebug("can not save photo image");
        file.write(buf4);
        file.close();
    }
}

void Client::sendMessage(const QString &msg){
 // for debugging
 qDebug() << msg;
 QStringList message = msg.split(";");
 qDebug() << "---Debugging messages from cpp---";
 qDebug() << message[0];
 qDebug() << message[1];
 qDebug() << message[2];
 qDebug() << "---Debugging messages from cpp---";

if (!message[0].isEmpty())
    _userName = message[0];

// message[0] is always the USERNAME
// the following code is for test purpose
   if (message[1]=="LOGINPASSWD"){
       if (message[2] == "aaa"){
           emit loginSuccess();
       }
       else
           emit loginFailed();
   }
  else if (message[1]=="GAMELIST"){
       //fake game list
       _gameList << "game1" << "game2" << "game3" << "game4";
       emit gameList(_gameList,_gameList.size());
   }
   if (message[1]=="CREATEGAME"){
       QString error = "";
       if (message[2].size() > 6)
           error = error + "GameID is no more than 6 characters\n";
       int time = message[3].toInt();
       if (time <= 60 || time >= 6000)
           error = error + "Time is no less than 60 seconds and larger than 6000 seconds\n";
       int noTeamA =  message[4].toInt();
       if (noTeamA <= 0 || noTeamA > 20)
           error = error + "Number of team A is no less than 0 and more than 20\n";
       int noTeamB = message[5].toInt();
       if (noTeamB <= 0 || noTeamB > 20)
           error = error + "Number of team B is no less than 0 and more than 20\n";

       // success or fail should be decided by server side as well
       if (error.isEmpty()) {
           _gameId = message[2];
           _gameTime = time;
           _noOfTeamA = noTeamA;
           _noOfTeamB = noTeamB;
           emit gameCreateSuccess(_gameId, _gameTime, _noOfTeamA, _noOfTeamB);
       }
       else{
           qDebug() << error;
            emit gameCreateFailed(error);
       }
   }
   if (message[1] == "JOINGAME") {
        _gameId= message[2];
        _gameTime = 100;
        _noOfTeamA = 11;
        _noOfTeamB = 10;
        QString joinUserName("test");
        emit joinGameInfo(_gameId, _gameTime, "0/5", "0/5", joinUserName,  _gameList.contains(_gameId));
   }
   if (message[1] == "LEAVEGAME") {
        _gameId = "";
   }
   if (message[1] == "JOINTEAM") {
        qDebug() << "--cpp--" << _gameId << "--cpp--";
        emit teamJoined(_gameId, _gameList.contains(_gameId));

          qDebug() << "a user joins game";
          emit joinGameInfo(_gameId, 100, "4/5", "4/5", "Liang", _gameList.contains(_gameId));
         // sleep(3);
          qDebug() << "a user leaves game";
          emit leaveGameInfo(_gameId, 100, "4/4", "4/5", "Le", _gameList.contains(_gameId));
          //sleep(3);
          //qDebug() << "game abort";
          //emit gameAbort();

   }
   if (message[1] == "GAMESTART") {
        emit startGame();
   }

   //test code
   if (message[1] == "GAMEUPDATE1")
       emit gameUpdate(_gameId, 100, "4/5", "4/5", "Tim", "Ali", 70, true);
   if (message[1] == "GAMEUPDATE2")
       emit gameUpdate(_gameId, 100, "3/5", "2/5", "Alice", "Bob", 80, false);
   if (message[1] == "ONTARGET1")
       emit onTarget(true, "Liang");
   if (message[1] == "ONTARGET2")
       emit onTarget(false, "");
   if (message[1] == "ENDGAME"){
       emit gameEnd();
       emit showResult("TeamA");
   }

}

// Will be tested after client connects to the server
QByteArray Client::loadPhoto(const QString &uName)
{
    if (!uName.isEmpty())
        _userName = uName;

    QString path("/home/user/MyDocs/DCIM/");
    QDir folder = QDir(path);
    folder.setFilter(QDir::Files | QDir::NoSymLinks);
    folder.setSorting(QDir::Time);
    QString filename = folder.entryList(QDir::Files | QDir::NoDotAndDotDot).at(0);
    filename = path.append(filename);
    QByteArray buf(_userName.toUtf8());
    QByteArray buf1(";LOGINPHOTO;");
    buf.append(buf1);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        qDebug("Can not open photo image");
    QByteArray buf2 = file.readAll();
    buf.append(buf2);
    return buf;
}

void Client::minimize(){
    QDBusConnection connection = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
    connection.send(message);
}
