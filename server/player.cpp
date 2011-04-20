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
    //TODO match in database
}

void Player::loginWithPicture(QString* uname, QImage* picture){
    //TODO recognize and login
}

void Player::logout(QString* uname){
    _logged = false;
    emit loggedOutSignal();
}

void Player::createGame(QString* uname, QString* game_id, int duration){
    if(GameFactory::exists(game_id)){
        emit gameCreatedSignal(false);
        return;
    }else{
        game* game = GameFactory::getGame(game_id);
        game->setCreator(uname);
        game->setDuration(duration);
        return;
    }
}

void Player::setTime(QString *game_id, QDate *date){
    if(GameFactory::exists(game_id)){
        game* game = GameFactory::getGame(game_id);
        if(game->getCreator()->compare(_name) == 0){
            game->setStartTime(date);
        }
    }
}

void Player::setDuration(QString *game_id, int duration){
    if(GameFactory::exists(game_id)){
        game* game = GameFactory::get()->getGame(game_id);
        if(game->getCreator()->compare(_name) == 0){
            game->setDuration(duration);
        }
    }
}

void Player::invite(QString* uname,QString* target_name,QString* game_id){
    //bool res = DataBaseHelper::sendInvite(uname, target_name, game_id);
   // emit playerInvitedSignal(res);
}

void Player::joinGame(QString* uname,QString* game_id){
    GameFactory *factory = GameFactory::get();
    game* game = factory->getGame(game_id);
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

void Player::left(QString *player, QString *team, QString *game_id){
    if(player->compare(_name) == 0){
        clearGameData();
        game* game = GameFactory::get()->getGame(game_id);
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
        if(damage >= _PLAYER_KILL_DAMAGE){
            (this->_kills)++;
        }
        emit hitSignal(true, damage, target);
    }else if(target->compare(_name) == 0){
        if(damage >= _PLAYER_KILL_DAMAGE){
            (this->_deaths)++;
        }
    }
}
