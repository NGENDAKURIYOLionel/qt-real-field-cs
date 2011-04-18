#include "game.h"

game::game(QObject *parent) :
        QObject(parent)
{
    _duration = -1; //marked as not set
    _start = NULL;
    _max_players = 10;
    _ended = false;
    _players = new QHash<QString,PlayerEntry*>();
    _timer = new QTimer(this);
    _timer.setSingleShot(true);
    connect(_timer,SIGNAL(timeout()), this, SLOT(endGame()));
    this->setImageRecognitionHelper(NULL);
    this->setDataBaseHelper(NULL);
}

game::~game(){
    //TODO auto generated destructor stub
}

time_t game::getStartTime(){
    return _start;
}

int game::getDuration(){
    return _duration;
}

int game::getMaxPlayers(){
    return _max_players;
}

int game::getNumberOfTeams(){
    return _teams;
}

bool game::setStartTime(time_t time){
    _start = time;
    return true;
}

bool game::setDuration(int duration){
    _duration = duration;
    //adjust the timer to the new duration
    _timer.setInterval(duration * 1000); //duration in milliseconds
    emit durationChanged();
    return true;
}

bool game::setMaxPlayers(int players){
    _max_players = players;
    emit maxPlayersChanged();
    return true;
}

bool game::setNumberOfTeams(int teams){
    _teams = teams;
    emit numberOfTeamsChanged();
    return true;
}

/*
 Join a team in this game
 */
bool game::joinTeam(int team, PlayerInfo* player){
    //check that a team with the given index exists and that the player isn't NULL
    if(team >= _teams || player == NULL){
        return false;
    }else{
        //return false if the player is already in this game
        if(_players->contains(player->getUID())){
            return false;
        }else{
            //create a new entry for the player and reset values in it
            PlayerEntry *entry = new PlayerEntry(player);
            entry->_death_count = 0;
            entry->_kill_count = 0;
            entry->_miss_count = 0;
            entry->_team = team;
            //add the player entry to the hash
            _players->insert(player->getUID(),entry);
            //emit a signal
            emit(joined(team,player));
            //return success
            return true;
        }
    }
}

/*
 Leave a currently joined game
 Returns false also if the player isn't currently in this game
 */
bool game::leaveGame(PlayerInfo* player){
    if(player == NULL){
        return false;
    }else{
        if(_players->remove(player->getUID()) > 0){
            emit left(player);
            return true;
        }else{
            return false;
        }
    }
}

/*
 Return the number of players in the given team
 */
int game::playersInTeam(int team){
    //no such team so can't have any players in it either
    if(team >= _teams){
        return 0;
    }else{
        //get the values as a list from the QHash
        QList<PlayerEntry*> list = _players->values();
        int size = list.size(), count = 0;
        //go through the values
        for(int i = 0; i < size; i++){
            //if the team index is the same up the count
            if(list.at(i)->_team == team){
                count++;
            }
        }
        return count;
    }
}
/*
 Starts this game
 */
void game::startGame(){
    _timer.start();
    _ended = false;
    emit gameStarted();
}
/*
 Ends this game
 */
void game::endGame(){
    _timer.stop();
    _ended = true;
    emit gameEnded();
}
/*
 Cancels/aborts the ongoing game and emits a corresponding signal
 Game also ends if it's canceled
 */
void game::cancelGame(){
    _timer.stop();
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

bool game::shot(QImage* image, PlayerInfo* player){
    if(image == NULL || player == NULL){
        return false;
    }
    if(1/*TODO match with imageRecognition */){
        QString hit_player_id = NULL;
        //TODO currently just kill_count the player
        int amount = 100;
        emit hit(player, hit_player_id, amount);
        PlayerEntry *entry = _players->value(player->getUID());
        entry->_kill_count = entry->_kill_count + 1;
        return true;
    }else{
        PlayerEntry *entry = _players->value(player->getUID());
        entry->_miss_count = entry->_miss_count + 1;
        emit miss(player);
        return false;
    }
}
