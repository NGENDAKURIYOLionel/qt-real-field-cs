#include "game.h"

game::game(QString *id, QObject *parent) :
        QObject(parent)
{
    _duration = -1; //marked as not set
    _start = NULL;
    _max_players = 10;
    _ended = false;
    _players = new QHash<QString*,QString*>();
    _teams = new QSet<QString*>();
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
    _game_id = id;
    connect(_timer,SIGNAL(timeout()), this, SLOT(endGame()));
    this->setImageRecognitionHelper(NULL);
    this->setDataBaseHelper(NULL);
    connect(this,SIGNAL(durationChanged()),this,SLOT(onGameChange()));
    connect(this,SIGNAL(joined(QString*,QString*)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(left(QString*,QString*)),this,SLOT(onGameChange()));
}

QDate* game::getStartTime(){
    return _start;
}

int game::getDuration(){
    return _duration;
}

int game::getMaxPlayers(){
    return _max_players;
}

int game::getNumberOfTeams(){
    return _teams->size();
}

bool game::setStartTime(QDate* time){
    _start = time;
    return true;
}

bool game::setDuration(int duration){
    _duration = duration;
    //adjust the timer to the new duration
    _timer->setInterval(duration * 1000); //duration in milliseconds
    emit durationChanged();
    return true;
}

bool game::setMaxPlayers(int players){
    _max_players = players;
    emit maxPlayersChanged();
    return true;
}

void game::addTeam(QString *team){
    _teams->insert(team);
}

/*
 Join a team in this game
 */
void game::joinTeam(QString* player,QString* team){
    if(*player == NULL || *team == NULL){
        return;
    }
    if(_players->keys().size() < getMaxPlayers()){
        _players->insert(player,team);
        emit joined(player,team);
    }
}
/*
 Leave the game
 */
void game::leaveGame(QString *id){
    QString *team = _players->value(id);
    if(_players->remove(id) > 0){
        emit left(id,team, _game_id);
    }
}

/*
 Return the number of players in the given team or -1 if no such team
 */
int game::playersInTeam(QString *team){
    if(_teams->contains(team)){
        return -1;
    }
    //get the values as a list from the QHash
    QList<QString*> list = _players->values();
    int size = list.size(), count = 0;
    //go through the values
    for(int i = 0; i < size; i++){
        //if the team index is the same up the count
        if(list.at(i)->compare(team)){
            count++;
        }
    }
    return count;
}
/*
 Starts this game
 */
void game::startGame(){
    _timer->start();
    _ended = false;
    emit gameStarted();
}
/*
 Ends this game
 */
void game::endGame(){
    _timer->stop();
    _ended = true;
    QString* win_team = getWinningTeam();
    QList<QString*> players = _players->keys();
    emit gameEnded(win_team,&players);
}
/*
 Cancels/aborts the ongoing game and emits a corresponding signal
 Game also ends if it's canceled
 */
void game::cancelGame(){
    _timer->stop();
    _ended = true;
    emit gameCanceled();
}
/*
 Returns true if this game has ended
 */
bool game::hasEnded(){
    return _ended;
}

/*
 Set this objects DataBaseHelper object
 */
void game::setDataBaseHelper(DataBaseHelper *helper){
    _DBH=helper;
}

/*
 Set this game objects ImageRecognitionHelper object
 */
void game::setImageRecognitionHelper(ImageRecognitionHelper *helper){
    _IRH = helper;
}

void game::shot(QImage* image, QString* id){
    if(1/*TODO match with imageRecognition */){
        QString *hit_player_id = NULL;
        //TODO currently just kills the player
        int amount = 100;
        emit hit(id, hit_player_id, amount);
        //TODO update player kill count
    }else{
        //TODO update player miss count
        emit miss(id);
    }
}

QString* game::getGameId(){
    return _game_id;
}

void game::onGameChange(){
    QHash<QString*, int> *hash = new QHash<QString*, int>();
    for(QSet<QString*>::const_iterator i = _teams->begin();i != _teams->end();i++){
        hash->insert((*i),this->playersInTeam((*i)));
    }
    emit gameInfo(_game_id,getDuration(),hash);
}

QString* game::getWinningTeam(){
    QHash<QString*, int> *hash= new QHash<QString*, int>();
    QList<QString*> list = _players->values();
    for(QList<QString*>::const_iterator i = list.begin(); i != list.end();i++){
        int val = hash->value((*i));
        hash->insert((*i), (val+ 1));
    }
    list = hash->keys();
    int alive = 0;
    QString* team;
    for(QList<QString*>::const_iterator i = list.begin();i != list.end();i++){
        int tmp = hash->value((*i));
        if(tmp > alive){
            alive = tmp;
            team = (*i);
        }
    }
    return team;
}
