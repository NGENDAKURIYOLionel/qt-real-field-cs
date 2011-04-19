#include "player.h"
#include "gamefactory.h"

Player::Player(QString* id,QObject *parent) :
    QObject(parent)
{
    _name = id;
    _in_game = false;
    _kills = 0;
    _deaths = 0;
    _misses = 0;
    _logged = false;
    _alive = true;
}

void Player::loginWithPassword(QString* uname,QString* password){

}

void loginWithPicture(QString* uname,QImage* picture);
void logout(QString* uname);
void gamelist(QString* uname);
void createGame(QString* uname, QString* gameId, int duration);
void setTime(QString* uname,QDate* date);
void setDuration(QString* uname,int duration);
void setPlayers(QString* uname,int players);
void setTeams(QString* uname,int teams);
void invite(QString* uname,QString* targetName,int gameId);
void Player::joinGame(QString* uname,QString* gameId){
    GameFactory *factory = GameFactory::get();
    game* game = factory->getGame(gameId);
    connect(this, SIGNAL(abortGameSignal()),game,SLOT(cancelGame()));
    connect(this, SIGNAL(endGameSignal()),game,SLOT(endGame()));
    connect(this, SIGNAL(startGameSignal()),game,SLOT(startGame()));
    connect(this, SIGNAL(joinTeamSignal(QString*,QString*)),game,SLOT(joinTeam(QString*,QString*)));
    connect(this, SIGNAL(leaveGameSignal(QString*)),game,SLOT(leaveGame(QString*)));
    connect(this, SIGNAL(shotSignal(QImage*,QString*)),game,SLOT(shot(QImage*,QString*)));

    connect(game,SIGNAL(gameStarted()),this,SLOT(gameStarted()));
    connect(game, SIGNAL(gameEnded(QString*, QList<Player*>*)),this, SLOT(gameEnded()));
    connect(game,SIGNAL(gameCanceled()), this, SLOT(gameAborted()));
    connect(game,SIGNAL(joined(QString*,QString*)), this, SLOT(joined(QString*, QString*)));
    connect(game,SIGNAL(miss(QString*)), this, SLOT(miss(QString*)));
    connect(game,SIGNAL(hit(QString*,QString*,int)), this, SLOT(hit(QString*,QString*,int)));
    connect(game, SIGNAL(destroyed()), this, SLOT(clearGameData()));
    connect(game, SIGNAL(left(QString*,QString*)), this, SLOT(left()));

    _in_game = true;
}

void Player::joinTeam(QString* uname,QString* teamId){
    emit joinTeamSignal(uname,teamId);
}

void Player::leave(QString* uname){
    if(uname->compare(_name)){
        emit leaveGameSignal(uname);
    }
}

void Player::cancel(QString* uname){
    if(uname->compare(_name)){
        emit abortGameSignal();
    }
}

void Player::shoot(QString* uname,QImage* picture){
    if(uname->compare(_name)){
       emit shotSignal(picture, uname);
    }
}

void Player::gameStart(QString* uname){
    if(uname->compare(_name)){
        emit startGameSignal();
    }
}

void Player::gameStarted(){
    _alive = true;
    //TODO maybe empty kills etc
    emit gameStartedSignal();
}

void Player::gameEnded(QString *win_team, QList<Player*> *players){
    emit gameEndedSignal(win_team,players);
}

void Player::gameAborted(){
    emit gameAbortedSignal();
}

void Player::clearGameData(){
    _in_game = false;
}

void Player::joined(QString* player, QString* team){
    if(player->compare(_name) == 0){
        emit joinedSignal(true);
    }
}

void Player::left(QString *player, QString *team, QString *gameid){
    if(player->compare(_name) == 0){
        clearGameData();
        game* game = GameFactory::get()->getGame(gameid);
        this->disconnect(game);
        game->disconnect(this);
    }
}

void Player::miss(QString* shooter){
    if(shooter->compare(_name) == 0){
        emit hitSignal(false,-1, NULL);
    }
}

void Player::hit(QString* shooter, QString* target, int damage){
    if(shooter->compare(_name) == 0){
        emit hitSignal(true, damage, target);
    }
}


