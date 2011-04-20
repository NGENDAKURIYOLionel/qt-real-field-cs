#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QtCore>
#include <QNetworkConfigurationManager>



class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    //void sendMessage(QString message);

    Q_INVOKABLE void connectto(QString ip, QString port);
    Q_INVOKABLE void sendMessage(const QString &message);
    Q_INVOKABLE void sendImage(const QByteArray &image);
    Q_INVOKABLE QByteArray loadPhoto(const QString &uName);
    Q_INVOKABLE void minimize();

public slots:
    void readMessage();

signals:
    void toGameLogin();
    void emitError(QString errorMessage);

    void login(QString errorMessage);
    void loginSuccess();
    void loginFailed();

    void gameList(QStringList list, int size);
    void gameCreateSuccess(QString gameId, int gameTime, int noOfTeamA, int noOfTeamB);
    void gameCreateFailed(QString error);
    void joinGameInfo(QString gameId, int gameTime, QString noOfTeamA, QString noOfTeamB, QString joinUserName, bool isGameInList);
    void leaveGameInfo(QString gameId, int gameTime, QString noOfTeamA, QString noOfTeamB, QString leaveUserName, bool isGameInList);
    void teamJoined(QString gameId, bool isGameInList);
    void startGame();
    void leaveGame();
    void gameAbort();

    void gameEnd();
    void showResult(QString winner);
    void gameUpdate(QString gameId, int gameTime, QString noOfTeamALeft, QString noOfTeamBLeft,
                    QString shooter, QString beShotOne, int health, bool isSelfKilled);
    void onTarget(bool hit, QString uName);

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
    void connected();

private:
    // variable for Network Connection
    QTcpSocket *tcpSocket;
    QString currentMessage;
    quint16 blockSize;
    QNetworkSession *networkSession;

    // variable for QML client
    QString _userName;
    QString _gameId;
    int _noOfTeamA;
    int _noOfTeamB;
    int _gameTime;
    QStringList _gameList;
};

#endif




/*
#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    Q_INVOKABLE void sendMessage(const QString &msg);
    Q_INVOKABLE void sendImage(const QByteArray &image);
    Q_INVOKABLE QByteArray loadPhoto(const QString &uName);
    Q_INVOKABLE void minimize();


signals:
    void login(QString errorMessage);
    void loginSuccess();
    void loginFailed();

    void gameList(QStringList list, int size);
    void gameCreateSuccess(QString gameId, int gameTime, int noOfTeamA, int noOfTeamB);
    void gameCreateFailed(QString error);
    void joinGameInfo(QString gameId, int gameTime, QString noOfTeamA, QString noOfTeamB, QString joinUserName, bool isGameInList);
    void leaveGameInfo(QString gameId, int gameTime, QString noOfTeamA, QString noOfTeamB, QString leaveUserName, bool isGameInList);
    void teamJoined(QString gameId, bool isGameInList);
    void startGame();
    void leaveGame();
    void gameAbort();

    void gameEnd();
    void showResult(QString winner);
    void gameUpdate(QString gameId, int gameTime, QString noOfTeamALeft, QString noOfTeamBLeft,
                    QString shooter, QString beShotOne, int health, bool isSelfKilled);
    void onTarget(bool hit, QString uName);


private:
    QString _userName;
    QString _gameId;
    int _noOfTeamA;
    int _noOfTeamB;
    int _gameTime;
    QStringList _gameList;
};

#endif
*/
