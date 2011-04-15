#ifndef CLI_H
#define CLI_H
#include <QString>
#include <QDebug>

class Cli : public QObject
{
    Q_OBJECT
public:
    Cli();

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
