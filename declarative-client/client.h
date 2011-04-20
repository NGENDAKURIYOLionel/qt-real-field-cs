#ifndef CLIENT_H
#define CLIENT_H

//#include <QDialog>
#include <QTcpSocket>
#include <QtCore>

//class QDialogButtonBox;
//class QLabel;
//class QLineEdit;
//class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    Q_INVOKABLE void sendMessage(const QString &msg);
    Q_INVOKABLE void sendImage(const QByteArray &image);
    Q_INVOKABLE QByteArray loadPhoto(const QString &uName);


public slots:
    void readMessage();
    void sendMessageSlot();

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





private slots:
    //void connectto();
    void displayError(QAbstractSocket::SocketError socketError);
    void enablereadLineButton();
    void enablesendLineButton();
    void enableconnectButton();
    void sessionOpened();
    void connected();

private:
    /*QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QPushButton *readLineButton;
    QPushButton *sendLineButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;*/

    QTcpSocket *tcpSocket;
    QString currentMessage;
    quint16 blockSize;

    void connectto();
    QNetworkSession *networkSession;


    QString _userName;
    QString _gameId;
    int _noOfTeamA;
    int _noOfTeamB;
    int _gameTime;
    QStringList _gameList;
};

#endif
