#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QImage>
#include <QDate>
#include <QHash>
#define _PLAYER_KILL_DAMAGE 100

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString *id, QObject *parent = 0);
    bool loggedIn();

signals:
    //Game side
    void startGameSignal();
    void endGameSignal();
    void abortGameSignal();
    void shotSignal(QImage* image, QString* player);
    void joinTeamSignal(QString* player, QString* team);
    void leaveGameSignal(QString* player);
    //MessageHandler side
    void loggedInSignal(bool);
    void loggedOutSignal();
    void gameCreatedSignal(bool);
    void gameInfoSignal(QString* gameid, int duration, QHash<QString*, int> *teams);
    void joinedSignal(bool);
    void gameStartedSignal();
    void gameAbortedSignal();
    void gameEndedSignal(QString *win_team, QList<Player*> *players);
    void hitSignal(bool value, int damage, QString* target);
    void updateSignal(QString *game, int time_left, QHash<QString*, int> *teams, int kills, bool alive);
    void updatePlayerStatusSignal(QString *killer, QString *killed);
    void playerInvitedSignal(bool);

public slots:
    void loginWithPassword(QString* uname,QString* password);
    void loginWithPicture(QString* uname,QImage* picture);
    void logout(QString* uname);
    void gamelist(QString* uname);
    void createGame(QString* uname, QString* gameId, int duration);
    void setTime(QString* gameId, QDate* date);
    void setDuration(QString* gameId, int duration);
    void invite(QString* uname,QString* targetName,QString *gameId);
    void joinGame(QString* uname,QString* gameId);
    void joinTeam(QString* uname,QString* teamId);
    void leave(QString* uname);
    void cancel(QString* uname);
    void shoot(QString* uname,QImage* picture);
    void gameStart(QString* uname);
    void gameStarted();
    void gameEnded(QString *win_team, QList<Player*> *players);
    void gameAborted();
    void joined(QString* player, QString* team);
    void left(QString* player, QString* team, QString* game);
    //emitted if the shooter misses, contains shooter
    void miss(QString* shooter);
    //emits a signal containing shooter, targets id, damage as int from 1-100
    void hit(QString* shooter, QString* target, int damage);

private:
    void clearGameData();
    int _kills;
    int _deaths;
    int _misses;
    bool _logged;
    bool _alive;
    bool _in_game;
    QString *_name;
};

#endif // PLAYER_H
