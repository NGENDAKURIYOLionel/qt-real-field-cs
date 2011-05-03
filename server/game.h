#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <QSet>
#include <QDate>
#include "imagerecognitionhelper.h"
#include "server.h"

class Server;
class game : public QObject
{
    Q_OBJECT
public:
    explicit game(QString gameid, QObject *parent = 0,Server *s=NULL);
    QDate* getStartTime();
    bool setStartTime(QDate* time);
    int getDuration();
    bool setDuration(int);
    int getMaxPlayers();
    QString getCreator();
    void setCreator(QString creator);
    int playersInTeam(QString team);
    bool setMaxPlayers(int);
    int getNumberOfTeams();
    void addTeam(QString team);
    QString getGameId();
    bool hasEnded();
    QString getGameInfo();
	unsigned teamAplayers;
	unsigned teamBplayers;
public slots:
    void startGame();
    void endGame();
    void cancelGame();
    void shot(QByteArray* image, QString player);
    void joinTeam(QString player, QString team);
    void leaveGame(QString player);

private slots:
    void onGameChange();
    void onDelete();

signals:
    void gameStarted();
    void gameEnded(QString win_team, QList<QString> *players);
    void gameCanceled();
    void joined(QString player, QString team, int teamA, int teamB);
    void left(QString player, QString team, QString game, int teamA, int teamB);
    //emitted if the shooter misses, contains shooter
    void miss(QString shooter);
    //emits a signal containing shooter, targets id, damage as int from 1-100
    void hit(QString shooter, QString target, int damage);
    void gameInfo(QString gameid, int duration, QHash<QString, int> *teams);
    void gameUpdate(int nofAliveA, int totalA, int nofAliveB, int totalB, QString shooter, QString target, int health,bool alive);

private:
    QString getWinningTeam();
    int _duration;
    QDate *_start;
    int _max_players;
    bool _ended;
    QString _game_id;
    QString _creator;
    //HashMap of player/team - pairs
    QHash<QString, QString> *_players;
    QHash<QString, int> *_change_hash;
    QHash<QString, int> *_teams;
    QTimer *_timer;
    QString _last_hit_player;
    Server *server;

};

#endif // GAME_H
