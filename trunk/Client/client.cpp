#include <QtNetwork>
#include <QtCore>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include "client.h"

Client::Client(QWidget *parent)
:   QDialog(parent), networkSession(0), _userName(""), _gameId(""),nextBlockSize(0)
{
    // find out which IP to connect to
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(readMessage()));

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        networkSession->open();
    }
}

// functions needed for network connection
void Client::connectto(QString ip, QString port)
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(ip, port.toInt());
}

void Client::connected()
{
    emit toGameLogin();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
       emit emitError("The host was not found. Please check the host name and port settings.");
       break;
    case QAbstractSocket::ConnectionRefusedError:
        emitError("The connection was refused by the peer.");
        break;
    default:
        QString error("The following error occurred: ");
        error.append(tcpSocket->errorString());
        emitError(error);
    }
}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

// functions for QML client
void Client::minimize(){
    QDBusConnection connection = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
    connection.send(message);
}

void Client::readMessage()
{
//    QTextStream in(tcpSocket);
//    QString newMessage;
//    QStringList message;

//    newMessage = in.readAll();
//    message = newMessage.split(";");

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_7);
    if (nextBlockSize == 0 ) {
        if(tcpSocket->bytesAvailable() < sizeof(quint32))
            return;
        in >> nextBlockSize;
    }
    qDebug() << "nextBlocksize is: " << nextBlockSize;
    qDebug() << "bytesAvailable is "<< tcpSocket->bytesAvailable();
    if (tcpSocket->bytesAvailable() < nextBlockSize)
        return;
    nextBlockSize = 0;
    QStringList message;
    QString newMessage;
    in >> newMessage;

    qDebug() << newMessage;
    //sendMessage("test;LOGINPASSWD;aaa");
    message = newMessage.split(";");
    // for debugging
    qDebug() << "---Debugging from readMessage starts---";
    qDebug() << newMessage << message.size();
    foreach(QString msg, message)
        qDebug() << msg;
    qDebug() << "---Debugging from readMessage ends---";

    if (message.size() == 1)
        return;

    if (!message[0].isEmpty())
       _userName = message[0];

   // message[0] is always the USERNAME
      if (message[1]=="LOGIN"){
          if (message[2] == "true"){
              emit loginSuccess();
          }
          else if (message[2] == "false")
              emit loginFailed();
      }
     if (message[1]=="GAMELIST"){
          for (int i = 2; i < message.size()-1; i++)
              _gameList.append(message[i]);
          emit gameList(_gameList, _gameList.size());
      }
      if (message[1]=="GAMECREATED"){
          if (message[2] == "true"){
              emit gameCreateSuccess(_gameId, _gameTime, _noOfTeamA, _noOfTeamB);
          }
          else if (message[2] == "false")
             emit gameCreateFailed("Game setting error");
      }
      if (message[1] == "GAMEINFO" && message.size() == 6) {
           _gameId= message[2];
           emit joinGameInfo(message[2], message[3].toInt(), message[4], message[5], QString("you"),  _gameList.contains(_gameId));
      }
      if (message[1] == "TEAMJOINED") {
           emit teamJoined(_gameId, _gameList.contains(_gameId));
      }
      if (message[1] == "GAMESTARTED") {
           emit startGame();
      }
      if (message[1] == "GAMESABORT") {

      }
      if (message[1] == "USERJOIN" && message.size() == 7) {
          emit joinGameInfo(message[2], message[3].toInt(), message[4], message[5], message[6],  _gameList.contains(_gameId));
      }
      if (message[1] == "USERLEAVE" && message.size() == 7) {
          emit leaveGameInfo(message[2], message[3].toInt(), message[4], message[5], message[6],  _gameList.contains(_gameId));
      }
      if (message[1] == "GAMEUPDATE" && message.size() == 10) {
          qDebug() << "gameupdate";
          emit gameUpdate(message[2], message[3].toInt(), message[4], message[5], message[6],
                          message[7], message[8].toInt(), message[9]);
      }
      if (message[1] == "ONTARGET"){
          if (message[2] == "true")
              emit onTarget(true, message[3]);
          else
             emit onTarget(false, "");
      }
      if (message[1] == "GAMEEND"){
          emit gameEnd();
          emit showResult(message[2]);
      }
}

//lewang;USERJOIN;a;100;4/5;3/5;lewang;
//lewang;USERLEAVE;a;100;4/5;3/5;lewang;
//lewang;GAMEUPDATE;a;100;4/5;1/5;tim;tom;50;true;
//lewnag;ONTARGET;true;a;  lewang;ONTARGET;false;

void Client::sendMessage(QString message){
    QStringList messageList;

    messageList = message.split(";");

    qDebug() << "---Debugging from sendMessage starts ---";
    qDebug() << message;
    foreach(QString msg, messageList)
        qDebug() << msg;

    if (!messageList[0].isEmpty())
       _userName = messageList[0];
    else
       message.insert(0, _userName);
    qDebug() << message;
    qDebug() << "---Debugging from sendMessage ends ---";

    if (messageList[1]=="CREATEGAME"){
        QString error = "";
        if (messageList[2].size() > 6)
            error = error + "GameID is no more than 6 characters\n";
        int time = messageList[3].toInt();
        if (time <= 60 || time >= 6000)
            error = error + "Time is no less than 60 seconds and larger than 6000 seconds\n";
        int noTeamA =  messageList[4].toInt();
        if (noTeamA <= 0 || noTeamA > 20)
            error = error + "Number of team A is no less than 0 and more than 20\n";
        int noTeamB = messageList[5].toInt();
        if (noTeamB <= 0 || noTeamB > 20)
            error = error + "Number of team B is no less than 0 and more than 20\n";

        // success or fail should be decided by server side as well
        if (error.isEmpty()) {
            _gameId = messageList[2];
            _gameTime = time;
            _noOfTeamA = noTeamA;
            _noOfTeamB = noTeamB;
        }
        else{
            qDebug() << error;
             emit gameCreateFailed(error);
        }
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(0) << message;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
    qDebug() << "size of the message:" << block.size() - sizeof(quint32);
    tcpSocket->write(block);

    //tcpSocket->write(message.toAscii());
}

void Client::sendImage(const QString &uName)
{
    //make sure image is stored before sending out
    sleep(2);
    if (!uName.isEmpty())
        _userName = uName;

    QString path("/home/user/MyDocs/DCIM/");
    QDir folder = QDir(path);
    folder.setFilter(QDir::Files | QDir::NoSymLinks);
    folder.setSorting(QDir::Time);
    QString filename = folder.entryList(QDir::Files | QDir::NoDotAndDotDot).at(0);
    filename = path.append(filename);

    QByteArray buf(_userName.toUtf8());
    QByteArray buf1;
    QFile *file;
    if (!uName.isEmpty()) {
        buf1.append(";LOGINPHOTO;");
         file = new QFile(filename);
    }
    else{
        buf1.append(";SHOOT;");
        resize(filename);
        file = new QFile("/home/user/MyDocs/DCIM/resized.jpg");
    }
    buf.append(buf1);

    if (!file->open(QIODevice::ReadOnly))
        qDebug("Can not open photo image");
    QByteArray buf2 = file->readAll();

    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(0) << QString(buf1) << buf2;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
    qDebug() << "size of the message:" << block.size() - sizeof(quint32);
    tcpSocket->write(block);

    //    QByteArray buf1 = image.left(image.indexOf(";"));//userName
    //    QByteArray buf2 = image.right(image.size() - image.indexOf(";") - 1);
    //    QByteArray buf3 = buf2.left(buf2.indexOf(";")); //LOGINPHOTO or SHOOT
    //    QByteArray buf4 = buf2.right(buf2.size() - buf2.indexOf(";") - 1); //Image
}

bool Client::resize(QString path)
{
   QImage image;
   if (!image.load(path)) {
       qDebug("failed to load image");
       return false;
   }

   QImage ret = image.scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   return (ret.save("/home/user/MyDocs/DCIM/resized.jpg",0,80));
}
