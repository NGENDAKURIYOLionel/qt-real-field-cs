#include <QtNetwork>
#include <QtCore>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include "client.h"

Client::Client(QWidget *parent)
:   QDialog(parent), networkSession(0), _userName(""), _gameId("")
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


void Client::readMessage()
{
    QTextStream in(tcpSocket);
    QString newMessage;
    QStringList message;

    newMessage = in.readAll();
    message = newMessage.split(";");

    // for debugging
    qDebug() << "---Debugging messages from cpp---";
    qDebug() << newMessage;
    foreach(QString msg, message)
        qDebug() << msg;
    qDebug() << "---Debugging messages from cpp---";

   if (!message[0].isEmpty())
       _userName = message[0];

   // message[0] is always the USERNAME
   // the following code is for test purpose
      if (message[1]=="LOGIN"){
          if (message[2] == "true"){
              emit loginSuccess();
          }
          else if (message[2] == "false")
              emit loginFailed();
      }
     if (message[1]=="GAMELIST"){
          foreach(QString msg, message)
            _gameList << msg;
          emit gameList(_gameList,_gameList.size());
      }
      if (message[1]=="GAMECTEATED"){
          if (message[2] == "true"){
              emit gameCreateSuccess(_gameId, _gameTime, _noOfTeamA, _noOfTeamB);
          }
          else if (message[2] == "false")
             emit gameCreateFailed("Game setting error");
      }
      if (message[1] == "GAMEINFO") {
           _gameId= message[2];
           emit joinGameInfo(message[2], message[3], message[4], message[5], "you",  _gameList.contains(_gameId));
      }
      if (message[1] == "TEAMJOINED") {
           emit teamJoined(_gameId, _gameList.contains(_gameId));
      }
      if (message[1] == "GAMESTARTED") {
           emit startGame();
      }
      if (message[1] == "GAMESABORT") {

      }
      if (message[1] == "USERJOIN") {
          emit joinGameInfo(message[2], message[3], message[4], message[5], "you",  _gameList.contains(_gameId));
      }
      if (message[1] == "USERLEAVE") {
          emit joinGameInfo(message[2], message[3], message[4], message[5], "you",  _gameList.contains(_gameId));
      }
      if (message[1] == "GAMEUPDATE") {
          emit gameUpdate(message[2], message[3].toInt(), message[4], message[5], message[6],
                          message[7], message[8].toInt(), message[9]);
      }
      if (message[1] == "ONTARGET"){
          if (message[2] == "true")
              emit onTarget(true, message[2]);
          else
             emit onTarget(false, "");
      }
      if (message[1] == "GAMEEND"){
          emit gameEnd();
          emit showResult(message[2]);
      }
}

void Client::sendMessage(const QString &message){
    QString newMessage;
    QStringList message;

    newMessage = in.readAll();
    message = newMessage.split(";");

    // for debugging
    qDebug() << "---Debugging messages from sendMessage ---";
    qDebug() << newMessage;
    foreach(QString msg, message)
        qDebug() << msg;
    qDebug() << "---Debugging messages from sendMessage ---";

    if (!message[0].isEmpty())
       _userName = message[0];

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
            _gameId = messageParts[2];
            _gameTime = time;
            _noOfTeamA = noTeamA;
            _noOfTeamB = noTeamB;
        }
        else{
            qDebug() << error;
             emit gameCreateFailed(error);
        }
    }


    tcpSocket->write(message.toAscii());
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

    //tcpSocket->write();
}
