#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QDate>
#include <QHash>
#include <QByteArray>
#include "server.h"
#define _PLAYER_KILL_DAMAGE 100

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString *id, QObject *parent = 0,Server *s=NULL);
    bool loggedIn();
    bool inGame();
    int  health;
    bool _alive;

signals:
    //Game side
    void startGameSignal();
    void endGameSignal();
    void abortGameSignal();
    void shotSignal(QByteArray* image, QString* player);
    void joinTeamSignal(QString* player, QString* team);
    void leaveGameSignal(QString* player);
    //MessageHandler side
    void loggedInSignal(QString message);
    void loggedOutSignal(QString *player);
    void gameCreatedSignal(QString message);
    void gameInfoSignal(QString message);
    void joinedSignal(QString message);
    void gameStartedSignal(QString message);
    void gameAbortedSignal(QString message);
    void gameEndedSignal(QString message);
    void hitSignal(QString message);
    void updateSignal(QString message);
    void updatePlayerStatusSignal(QString message);
    void playerInvitedSignal(QString message);
    void gameUpdateSignal(QString message);
    void leftSignal(QString message);

public slots:
    void loginWithPassword(QString* uname,QString* password);
    void loginWithPicture(QString* uname,QByteArray* picture);
    void logout(QString* uname);
    void createGame(QString* uname, QString* game_id, int duration);
    void setTime(QString* game_id, QDate* date);
    void setDuration(QString* game_id, int duration);
    void invite(QString* uname,QString* targetName,QString *game_id);
    void joinGame(QString* uname,QString* game_id);
    void joinTeam(QString* uname,QString* team_id);
    void leave(QString* uname);
    void cancel(QString* uname);
    void shoot(QString* uname,QByteArray* picture);
    void gameStart(QString* uname);
    void gameStarted();
    void gameEnded(QString *win_team, QList<Player*> *players);
    void gameAborted();
    void joined(QString* player, QString* team, int teamA, int teamB);
    void left(QString* player, QString* team, QString* game, int teamA, int teamB);
    //emitted if the shooter misses, contains shooter
    void miss(QString* shooter);
    //emits a signal containing shooter, targets id, damage as int from 1-100
    void hit(QString* shooter, QString* target, int damage);
    void gameUpdate(int nofAliveA, int totalA, int nofAliveB, int totalB, QString* shooter,
                    QString* target, int health, bool alive);


private:
    void clearGameData();
    int _kills;
    int _deaths;
    int _misses;
    bool _logged;
    bool _in_game;
    QString *_name;
    Server *server;
};

#endif // PLAYER_H
