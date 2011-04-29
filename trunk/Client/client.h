#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QtCore>
#include <QTimer>
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
    Q_INVOKABLE void sendMessage(QString message);
    Q_INVOKABLE void sendImage(const QString &uName);
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
    void gameUpdate(QString noOfTeamALeft, QString noOfTeamBLeft,
                    QString shooter, QString beShotOne, int health, QString isSelfKilled);
    void gameTime(QString remainingTime);
    void onTarget(bool hit, QString uName);

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
    void connected();
    void update();

private:
    void startGameTimer();
    void stopGameTimer();
    QTimer *_timer;
    int _countDown;

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

    QString _alive;

    QByteArray resize(QString path);
    quint32 nextBlockSize;
};

#endif


