#include "game.h"
#include "imagerecognitionhelper.h"
#include <string>

game::game(QString *id, QObject *parent) :
        QObject(parent)
{
    _duration = -1; //marked as not set
    _start = NULL;
    _max_players = 10;
    _ended = false;
    _players = new QHash<QString*,QString*>();
    _change_hash = new QHash<QString*, int>();
    _teams = new QSet<QString*>();
    _timer = new QTimer(this);
    _last_hit_player = new QString();
    _timer->setSingleShot(true);
    _game_id = id;
    connect(_timer,SIGNAL(timeout()), this, SLOT(endGame()));
    connect(this,SIGNAL(durationChanged()),this,SLOT(onGameChange()));
    connect(this,SIGNAL(joined(QString*,QString*)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(left(QString*,QString*)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(destroyed()),this,SLOT(onDelete()));
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

QString* game::getCreator(){
    return _creator;
}

void game::setCreator(QString *creator){
    _creator = creator;
}

bool game::setStartTime(QDate* time){
    _start = time;
    return true;
}

bool game::setDuration(int duration){
    _duration = duration;
    //adjust the timer to the new duration
    _timer->setInterval(duration * 1000); //duration in milliseconds
    return true;
}

bool game::setMaxPlayers(int players){
    _max_players = players;
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

void game::shot(QByteArray* image, QString* player){
    try{
        extern ImageRecognitionHelper irh;
		std::string temp_image(image->constData(), image->size());
		std::string temp_player(_last_hit_player->toAscii().constData());
        int amount = irh.match_all(temp_player,temp_image);
        if(amount >= 0){
            emit hit(player, _last_hit_player, amount);
        } else {
			emit miss(player);
		}
    }catch(errors_e *e){
        std::cout << "ERROR MESSAGE: " << __FUNCTION__ << " : ImageRecognitionHelper raised an error: " << e << '\n';
        emit miss(player);
    }
}

QString* game::getGameId(){
    return _game_id;
}

void game::onGameChange(){
    QHash<QString*, int> *hash = _change_hash;
    hash->clear();
    for(QSet<QString*>::const_iterator i = _teams->begin();i != _teams->end();i++){
        hash->insert((*i),this->playersInTeam((*i)));
    }
    emit gameInfo(_game_id,getDuration(),hash);
}

void game::onDelete(){
    delete _teams;
    delete _change_hash;
    delete _players;
    delete _timer;
    delete _last_hit_player;
}

QString* game::getWinningTeam(){
    QHash<QString*, int> *hash= _change_hash;
    QList<QString*> list = _players->values();
    for(QList<QString*>::const_iterator i = list.begin(); i != list.end();i++){
        int val = hash->value((*i));
        hash->insert((*i), (val+ 1));
    }
    list = hash->keys();
    QString *team;
    int alive = 0;
    for(QList<QString*>::const_iterator i = list.begin();i != list.end();i++){
        int tmp = hash->value((*i));
        if(tmp > alive){
            alive = tmp;
            team = (*i);
        }
    }
    return team;
}
