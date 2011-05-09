#include <QtNetwork>
#include <QtCore>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include "client.h"

Client::Client(QWidget *parent)
:   QDialog(parent), networkSession(0), _userName(""), _gameId(""), _alive("true"), nextBlockSize(0), _isCreator(false)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
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
    while(tcpSocket->bytesAvailable() > 0) {



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
            _gameList.clear();
            for (int i = 2; i < message.size(); i++){
              if (message[i].isNull())
                  continue;
              else
                  _gameList.append(message[i]);
          }
          foreach(QString tmp, _gameList)
              qDebug() << tmp;

          emit gameList(_gameList, _gameList.size());
      }
      if (message[1]=="GAMECREATED"){
          if (message[2] == "true"){
			  QString readableTime;
	          readableTime.sprintf("%02d:%02d", _gameTime / 60, _gameTime % 60);
                  _isCreator = true;
              emit gameCreateSuccess(_gameId, readableTime, _noOfTeamA, _noOfTeamB);
          }
          else if (message[2] == "false")
             emit gameCreateFailed("Game setting error");
      }
      if (message[1] == "GAMEINFO"){// && message.size() == 6) {
           _gameId= message[2];
           _gameTime = message[3].toInt();

		   QString readableTime;
	       readableTime.sprintf("%02d:%02d", _gameTime / 60, _gameTime % 60);
           emit joinGameInfo(message[2], readableTime, message[4], message[5], QString("you"),  _gameList.contains(_gameId));
      }
      if (message[1] == "TEAMJOINED") {
           emit teamJoined(_gameId, _gameList.contains(_gameId));
      }
      if (message[1] == "GAMESTARTED") {
           startGameTimer();
           emit startGame();
      }
      if (message[1] == "GAMESABORT") {
           emit gameEnd();
           emit showResult("Game Aborted");
      }
      if (message[1] == "USERJOIN"){// && message.size() == 6) {
          QString readableTime;
          readableTime.sprintf("%02d:%02d", _gameTime / 60, _gameTime % 60);
          emit joinGameInfo(_gameId, readableTime, message[2], message[3], message[4],  _gameList.contains(_gameId));
      }
      if (message[1] == "USERLEAVE"){// && message.size() == 6) {
          QString readableTime;
          readableTime.sprintf("%02d:%02d", _gameTime / 60, _gameTime % 60);
          emit leaveGameInfo(_gameId, readableTime, message[2], message[3], message[4],  _gameList.contains(_gameId));
      }
      if (message[1] == "GAMEUPDATE"){// && message.size() == 10) {
          qDebug() << "gameupdate";
          qDebug() << message[2] << message[3] << message[4] << message[5] << message[6] << message[7] << message[8];
          int remainingHealth;
          if (message[8].toInt() <= 0)
              remainingHealth = 0;
          else
              remainingHealth = message[8].toInt();
          if (message[7] == _userName && remainingHealth == 0)
              _alive = "false";

          QString noTeamALeft, noTeamBLeft;
          noTeamALeft.append(message[2]).append("/").append(message[3]);
          noTeamALeft.append(message[4]).append("/").append(message[5]);
          emit gameUpdate(noTeamALeft, noTeamBLeft, message[6], message[7], remainingHealth, _alive);
      }
      if (message[1] == "ONTARGET"){
          if (message[2] == "true")
              emit onTarget(true, message[3]);
          else
             emit onTarget(false, "");
      }
      if (message[1] == "GAMEEND"){
          if (!_isCreator)
            stopGameTimer();
          emit gameEnd();
          if (message[2] == "tie")
            emit showResult("Draw. Try again?");
          else
            emit showResult(message[2].append(" Won"));
      }
      }
}

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
        if (time <= 10 || time >= 6000)
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
             return;
        }
    }

     if (messageList[1]=="LEAVEGAME"){
         message.append(_gameId);
     }


    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(0) << message;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
    qDebug() << "size of the message:" << block.size() - sizeof(quint32);
    tcpSocket->write(block);
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

       QString message(_userName);
       QByteArray image;

       if (!uName.isEmpty()) {
           message.append(";LOGINPHOTO;");
           QFile file(filename);
           if (!file.open(QIODevice::ReadOnly))
               qDebug("Can not open photo image");
           image = file.readAll();
       }
       else{
           message.append(";SHOOT;");
           image = resize(filename);
           //file = new QFile("/home/user/MyDocs/DCIM/resized.jpg");
       }

       if(image.size()==0) {
           qDebug("send image abort");
            return;
       }

       QByteArray block;
       QDataStream out(&block, QIODevice::ReadWrite);
       out.setVersion(QDataStream::Qt_4_7);
       out << quint32(0) << message << image;
       out.device()->seek(0);
       out << quint32(block.size() - sizeof(quint32));
       qDebug() << "size of the message:" << block.size() - sizeof(quint32);
       tcpSocket->write(block);
}

QByteArray Client::resize(QString path)
{
   QImage image, resized;
   QByteArray resizedImage;
   QBuffer buf(&resizedImage);
   buf.open(QIODevice::WriteOnly);
   if (!image.load(path)) {
       qDebug("resize: failed to load image");
       return resizedImage;
   }
   resized = image.scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   resized.save(&buf,"JPG",80);
   return resizedImage;
}


void Client::update() {
    QString readableTime;
     _countDown--;
    qDebug() << __FILE__ << __LINE__ << __func__ << _countDown;
    readableTime.sprintf("%02d:%02d", _countDown / 60, _countDown % 60);

    if (_countDown == 0) {
       stopGameTimer();
       emit gameTime("00:00");
    }
    else {
       emit gameTime(readableTime);
    }
}


void Client::startGameTimer(){
    _countDown = _gameTime;
    update();
    _timer->start(1000);
}

void Client::stopGameTimer(){
    _countDown = 0;
    _timer->stop();
    //Game creator sends TIMEOVER message when game ends
    if (_isCreator)
        sendMessage(_userName+";"+"TIMEOVER");
}
