#ifndef CLIENT_H
#define CLIENT_H
#include <QString>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();

    Q_INVOKABLE void sendMessage();
signals:
        void loginFailed();

public slots:
void send();
//    void stop();

private:
    bool m_running;
};

#endif // CLIENT_H
