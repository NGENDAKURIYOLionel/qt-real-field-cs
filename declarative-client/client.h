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
    //Q_INVOKABLE int getListSize(const QStringList &list);


public slots:
    void readMessage();
    void sendMessageSlot();

signals:
    void login(QString errorMessage);
    void loginSuccess();
    void loginFailed();
    void gameList(QStringList list, int size);
    void gameCreateSuccess();
    void gameCreateFailed(QString error);
    void joinGameInfo(QString gameId, int gameTime, int noOfTeamA, int noOfTeamB);





   // void gameList(QString gameList,QString errorMessage);
   // void gameList(QString gameList);
    void lobby(QString gameInformation,QString errorMessage);
    void lobby(QString gameInformation);
    void gameState(QString gameState);
    void update(QString gameUpdate);

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
    QByteArray photo;
    void connectto();
    QNetworkSession *networkSession;
    QString userName;
};

#endif
