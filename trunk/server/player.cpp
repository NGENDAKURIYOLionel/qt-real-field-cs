#include "player.h"
#include "gamefactory.h"
#include <QByteArray>
#include "imagerecognitionhelper.h"
#include "DataBaseHelper.h"

Player::Player(QString id,QObject *parent,Server *s) :
    QObject(parent)
{
    _name = id;
    _in_game = false;
    _kills = 0;
    _deaths = 0;
    _misses = 0;
    _logged = false;
    _alive = true;
    server = s;
    health = 100;
}

bool Player::loggedIn(){
    return _logged;
}

bool Player::inGame(){
    return _in_game;
}

void Player::loginWithPassword(QString uname,QString password){
    qDebug("Player::loginwihPassword");
    std::string passwd = server->db->getPassword((uname).toStdString());
    //std::string passwd = "aaa";
    qDebug("test");
    if(passwd == password.toStdString()){
        cout<<"Login with password works!"<<endl;
        _logged = true;
        emit loggedInSignal(QString("LOGIN;true"));
    }else{
        cout<<"Login with password did not work!"<<endl;
        emit loggedInSignal(QString("LOGIN;false"));
    }
}

void Player::loginWithPicture(QString uname, QByteArray* picture) {
	extern ImageRecognitionHelper irh;
	std::string response_uid;
	std::string jpeg_image(picture->constData(), picture->size());
	// match_all() searches all registered UIDs
    if(irh.match_all(response_uid, jpeg_image) >= 0){
		// response_uid has a valid uid
        _logged = true;
        emit loggedInSignal(QString("LOGIN;true"));
    }else{
		// response_uid is empty
        emit loggedInSignal(QString("LOGIN;false"));
    }
}

void Player::logout(QString uname){
    _logged = false;
    emit loggedOutSignal(_name);
}

void Player::createGame(QString uname, QString game_id, int duration){
    if(GameFactory::exists(game_id)){
        emit gameCreatedSignal("GAMECREATED;false");
        return;
    }else{
        game* game = GameFactory::getGame(game_id);
        game->setCreator(uname);
        game->setDuration(duration);
        emit gameCreatedSignal("GAMECREATED;true");
        joinGame(uname,game_id);
        return;
    }
}

void Player::setTime(QString game_id, QDate *date){
    if(GameFactory::exists(game_id)){
        game* game = GameFactory::getGame(game_id);
        if(game->getCreator().compare(_name) == 0){
            game->setStartTime(date);
        }
    }
}

void Player::setDuration(QString game_id, int duration){
    if(GameFactory::exists(game_id)){
        game* game = GameFactory::getGame(game_id);
        if(game->getCreator().compare(_name) == 0){
            game->setDuration(duration);
        }
    }
}

void Player::invite(QString uname,QString target_name,QString game_id){
    //TODO invites to database and emit to players
    //bool res = DataBaseHelper::sendInvite(uname, target_name, game_id);
    //if(res){
    //  emit playerInvitedSignal(uname,target, game);
    //}
}

void Player::joinGame(QString uname,QString game_id){
    if(GameFactory::exists(game_id)){
        qDebug() << "joingame in player" << game_id;
        game* game = GameFactory::getGame(game_id);
        connect(this, SIGNAL(abortGameSignal()),game,SLOT(cancelGame()));
        connect(this, SIGNAL(endGameSignal()),game,SLOT(endGame()));
        connect(this, SIGNAL(startGameSignal()),game,SLOT(startGame()));
        connect(this, SIGNAL(joinTeamSignal(QString,QString)),game,SLOT(joinTeam(QString,QString)));
        connect(this, SIGNAL(leaveGameSignal(QString)),game,SLOT(leaveGame(QString)));
        connect(this, SIGNAL(shotSignal(QByteArray*,QString)),game,SLOT(shot(QByteArray*,QString)));

        connect(game,SIGNAL(gameStarted()),this,SLOT(gameStarted()));
        connect(game, SIGNAL(gameEnded(QString,QList<QString>*)),this, SLOT(gameEnded(QString,QList<QString>*)));
        connect(game,SIGNAL(gameCanceled()), this, SLOT(gameAborted()));
        connect(game,SIGNAL(joined(QString,QString,int,int)), this, SLOT(joined(QString,QString,int,int)));
        connect(game,SIGNAL(miss(QString)), this, SLOT(miss(QString)));
        connect(game,SIGNAL(hit(QString,QString,int)), this, SLOT(hit(QString,QString,int)));
        //connect(game, SIGNAL(destroyed()), this, SLOT(clearGameData()));
        connect(game, SIGNAL(left(QString,QString,QString,int,int)), this, SLOT(left(QString,QString,QString,int,int)));
        connect(game, SIGNAL(gameUpdate(int,int,int,int,QString,QString,int,bool)), this,
                SLOT(gameUpdate(int,int,int,int,QString,QString,int,bool)));

        _in_game = true;
        emit gameInfoSignal(game->getGameInfo());
    }
}

void Player::joinTeam(QString uname,QString teamId){
    qDebug() << GameFactory::getGameIds().size();
    emit joinTeamSignal(uname,teamId);
    qDebug() << "emit done, thread:" << thread();
}

void Player::leave(QString uname){
    if(uname.compare(_name)){
        emit leaveGameSignal(uname);
    }
}

void Player::cancel(QString uname){
    if(uname.compare(_name)){
        emit abortGameSignal();
    }
}

void Player::shoot(QString uname,QByteArray* picture){
    if(uname.compare(_name)){
       emit shotSignal(picture, uname);
    }
}

void Player::gameStart(QString uname){
    if(uname.compare(_name)){
        emit startGameSignal();
    }
}

void Player::gameStarted(){
    _alive = true;
    //TODO maybe empty kills etc
    emit gameStartedSignal("GAMESTARTED;");
}

void Player::gameEnded(QString win_team, QList<QString> *players){
    //emit gameEndedSignal(win_team,players);
    QString temp("GAMEEND;");
    temp.append(win_team);
    emit gameEndedSignal(temp);
}

void Player::gameAborted(){
    emit gameAbortedSignal("GAMEABORT;");
}

void Player::clearGameData(){
    _in_game = false;
}

void Player::joined(QString player, QString team, int teamA, int teamB){
    qDebug() << "player emitting joined name" << _name;
    qDebug() << "player emitting joined player" << player ;
    if(player.compare(_name) == 0){
        qDebug() << "real emit from player";
        emit joinedSignal("TEAMJOINED;");

    }
    emit joinedSignal(QString::number(teamA) + QString::number(teamB) + player);
}

void Player::left(QString player, QString team, QString game_id, int teamA, int teamB){
    if(player.compare(_name) == 0){
        clearGameData();
        game* game = GameFactory::getGame(game_id);
        this->disconnect(game);
        game->disconnect(this);
    }

    emit leftSignal(QString::number(teamA) + QString::number(teamB) + player);
}

void Player::miss(QString shooter){
    if(shooter.compare(_name) == 0){
        //emit hitSignal(false,-1, NULL);
        emit hitSignal("ONTARGET;false");
    }
}

void Player::hit(QString shooter, QString target, int damage){
    if(shooter.compare(_name) == 0){
        if(damage >= _PLAYER_KILL_DAMAGE){
            (this->_kills)++;

        }
        //emit hitSignal(true, damage, target);
        QString temp("ONTARGET;true;");
        emit hitSignal(temp.append(target));
    }else if(target.compare(_name) == 0){
        if(damage >= _PLAYER_KILL_DAMAGE){
            (this->_deaths)++;
        }
    }
}

void Player::gameUpdate(int nofAliveA, int totalA, int nofAliveB, int totalB, QString shooter, QString target, int health, bool alive) {

    QString tmp("GAMEUPDATE;");
    tmp.append(QString::number(nofAliveA) + QString(";"));
    tmp.append(QString::number(totalA) + QString(";"));
    tmp.append(QString::number(nofAliveB) + QString(";"));
    tmp.append(QString::number(totalB) + QString(";"));
    tmp.append(shooter + QString(";"));
    tmp.append(target + QString(";"));
    tmp.append(QString::number(health) + QString(";"));
    if(alive)
        tmp.append("true;");
    else
        tmp.append("false;");
    emit gameUpdateSignal(tmp);
}
