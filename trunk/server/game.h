#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QHash>
#include "PlayerInfo.h"
#include "imagerecognitionhelper.h"
#include "DataBaseHelper.h"
class PlayerEntry{
public:
    PlayerEntry(PlayerInfo*);
private:
    int _team;
    int _kill_count;
    int _death_count;
    int _miss_count;
    PlayerEntry *_player;
};

class game : public QObject
{
    Q_OBJECT
public:
    explicit game(QObject *parent = 0);
    ~game();
    time_t getStartTime();
    bool setStartTime(time_t);
    int getDuration();
    bool setDuration(int);
    int getMaxPlayers();
    int playersInTeam(int);
    bool setMaxPlayers(int);
    int getNumberOfTeams();
    bool setNumberOfTeams(int);
    bool joinTeam(int, PlayerInfo*);
    bool leaveGame(PlayerInfo*);
    void startGame();
    void cancelGame();
    bool hasEnded();
    bool shot(QImage*,PlayerInfo*);
    void setDataBaseHelper(DataBaseHelper*);
    void setImageRecognitionHelper(ImageRecognitionHelper*);

public slots:
    startGame();
    endGame();
    cancelGame();

signals:
    gameStarted();
    gameEnded();
    gameCanceled();
    durationChanged();
    maxPlayersChanged();
    numberOfTeamsChanged();
    joined(int,PlayerInfo*);
    left(PlayerInfo*);
    //emitted if the shooter misses, contains shooter
    miss(PlayerInfo*);
    //emits a signal containing shooter, targets id, damage as int from 1-100
    hit(PlayerInfo*, QString, int);

private:
    int _duration;
    time_t _start;
    int _max_players;
    bool _ended;
    QHash<QString,PlayerEntry*> *_players;
    QTimer _timer;
    ImageRecognitionHelper* _IRH;
    DataBaseHelper* _DBH;

};

#endif // GAME_H
