#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QHash>
#include <QSet>
#include <QDate>
#include "imagerecognitionhelper.h"
#include "DataBaseHelper.h"

class game : public QObject
{
    Q_OBJECT
public:
    explicit game(QString *gameid,QObject *parent = 0);
    QDate* getStartTime();
    bool setStartTime(QDate* time);
    int getDuration();
    bool setDuration(int);
    int getMaxPlayers();
    int playersInTeam(QString* team);
    bool setMaxPlayers(int);
    int getNumberOfTeams();
    void addTeam(QString* team);
    QString* getGameId();
    bool hasEnded();
    void setDataBaseHelper(DataBaseHelper*);
    void setImageRecognitionHelper(ImageRecognitionHelper*);

public slots:
    void startGame();
    void endGame();
    void cancelGame();
    void shot(QImage* image, QString* player);
    void joinTeam(QString* player, QString* team);
    void leaveGame(QString* player);
    void onGameChange();

signals:
    void gameStarted();
    void gameEnded(QString *win_team, QList<QString*> *players);
    void gameCanceled();
    void durationChanged();
    void maxPlayersChanged();
    void numberOfTeamsChanged();
    void joined(QString* player, QString* team);
    void left(QString* player, QString* team, QString* game);
    //emitted if the shooter misses, contains shooter
    void miss(QString* shooter);
    //emits a signal containing shooter, targets id, damage as int from 1-100
    void hit(QString* shooter, QString* target, int damage);
    void gameInfo(QString* gameid, int duration, QHash<QString*, int> *teams);

private:
    QString* getWinningTeam();
    int _duration;
    QDate *_start;
    int _max_players;
    bool _ended;
    QString *_game_id;
    //HashMap of player/team - pairs
    QHash<QString*, QString*> *_players;
    QSet<QString*> *_teams;
    QTimer *_timer;
    ImageRecognitionHelper* _IRH;
    DataBaseHelper* _DBH;

};

#endif // GAME_H
