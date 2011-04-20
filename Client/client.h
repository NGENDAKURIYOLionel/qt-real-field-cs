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
    void sendMessage(QString message);


public slots:
    void readMessage();
    void sendMessageSlot();

signals:
    void login(QString errorMessage);
    void login();
    void gameList(QString gameList,QString errorMessage);
    void gameList(QString gameList);
    void lobby(QString gameInformation,QString errorMessage);
    void lobby(QString gameInformation);
    void gameState(QString gameState);
    void update(QString gameUpdate);

private slots:
    void connectto();
    void displayError(QAbstractSocket::SocketError socketError);
    void enablereadLineButton();
    void enablesendLineButton();
    void enableconnectButton();
    void sessionOpened();
    void connected();

private:
    QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QPushButton *readLineButton;
    QPushButton *sendLineButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QTcpSocket *tcpSocket;
    QString currentMessage;
    quint16 blockSize;

    QNetworkSession *networkSession;
};

#endif
