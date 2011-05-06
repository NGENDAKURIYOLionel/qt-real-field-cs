#include "game.h"
#include "imagerecognitionhelper.h"
#include <string>
#include "playerfactory.h"
#include "server.h"

game::game(QString id, QObject *parent,Server *s) :
        QObject(parent)
{
    _duration = -1; //marked as not set
    _start = NULL;
    _max_players = 10;
    _ended = false;
    _players = new QHash<QString,QString>();
    _change_hash = new QHash<QString, int>();
    _teams = new QHash<QString, int>();
    _timer = new QTimer(this);
    _last_hit_player = "";
    //_timer->setSingleShot(true);
    _game_id = id;
    connect(_timer,SIGNAL(timeout()), this, SLOT(update()));
//    connect(this,SIGNAL(joined(QString,QString,int,int)),this,SLOT(onGameChange()));
//    connect(this, SIGNAL(left(QString,QString,QString,int,int)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(destroyed()),this,SLOT(onDelete()));
    server = s;
	addTeam("teamA");
	addTeam("teamB");
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
    return _teams->keys().size();
}

QString game::getCreator(){
    return _creator;
}

void game::setCreator(QString creator){
    _creator = creator;
}

bool game::setStartTime(QDate* time){
    _start = time;
    return true;
}

bool game::setDuration(int duration){
    _duration = duration;
    //adjust the timer to the new duration
   // _timer->setInterval(duration * 1000); //duration in milliseconds
    return true;
}

bool game::setMaxPlayers(int players){
    _max_players = players;
    return true;
}

void game::addTeam(QString team){
    _teams->insert(team, 0);
}

/*
 Join a team in this game
 */
void game::joinTeam(QString player,QString team) {
    qDebug() << "joinTeam in game" << thread();
    if(player == NULL || team == NULL){
        return;
    }
    if(_players->keys().size() < getMaxPlayers()) {
        if(!_teams->contains(team)) addTeam(team);
        //add player to players
        _players->insert(player,team);
        //add player to team, increments the player count
        _teams->insert(team, _teams->value(team) + 1);
        qDebug() << "game emitting joined";
        for(int i=0;i<_players->size();i++){
        PlayerFactory::getPlayer(_players->keys().at(i))->joined(player,team, playersInTeam("teamA"), playersInTeam("teamB"));
        }
    }
}
/*
 Leave the game
 */
void game::leaveGame(QString id){
	qDebug() << __FILE__ << __LINE__ << __func__;
    QString team = _players->value(id);
    if(_players->remove(id) > 0){
        int teams = _teams->value(team) - 1;
        if(teams < 0) teams = 0;
        _teams->insert(team, teams);

        int aTotal=0, bTotal=0;
        for(int i=0;i<_players->size();i++) {
            if(_players->value(_players->keys().at(i)).compare("teamA") == 0)
                aTotal++;
            else
                bTotal++;
        }
        for(int i=0;i<_players->size();i++){
        PlayerFactory::getPlayer(_players->keys().at(i))->left(id,team, _game_id, aTotal, bTotal);
        }
    }
}

/*
 Return the number of players in the given team or -1 if no such team
 */
int game::playersInTeam(QString team){
//	qDebug() << __FILE__ << __LINE__ << __func__ << team;
	Q_ASSERT(_teams->contains(team));
    return _teams->value(team);
}
/*
 Starts this game
 */
void game::startGame(){
        qDebug() << __FILE__ << __LINE__ << __func__ << _duration;
    _countdownTime = _duration;
    _timer->start(1000);
    _ended = false;
    for(int i=0;i<_players->size();i++){
    PlayerFactory::getPlayer(_players->keys().at(i))->gameStarted();
    }
}

/*
  Update timer
 */
void game::update(){
    _countdownTime--;
    qDebug() << "timer shoots" << _countdownTime;
    if (_countdownTime == 0)
        endGame();
}

/*
 Ends this game
 */
void game::endGame(){
    _timer->stop();
    _ended = true;
    QString win_team = getWinningTeam();
    QList<QString> players = _players->keys();
    for(int i=0;i<_players->size();i++){
    PlayerFactory::getPlayer(_players->keys().at(i))->gameEnded(win_team,&players);
    }
    GameFactory::destroyGame(_game_id);

}
/*
 Cancels/aborts the ongoing game and emits a corresponding signal
 Game also ends if it's canceled
 */
void game::cancelGame(){
    qDebug() <<"game cancel start";
    _timer->stop();
    _ended = true;
    for(int i=0;i<_players->size();i++){
        qDebug() <<"game loop "<<i;
    PlayerFactory::getPlayer(_players->keys().at(i))->gameAborted();
    }
    qDebug() <<"game cancel end";
}
/*
 Returns true if this game has ended
 */
bool game::hasEnded(){
    return _ended;
}

void game::shot(QByteArray* image, QString player){
	qDebug() << __FILE__ << __LINE__ << __func__ << "shoot slot " << image->size();
    try{
        extern ImageRecognitionHelper irh;
        std::string temp_image(image->constData(), image->size());
        std::string temp_player(_last_hit_player.toAscii().constData());
        std::string irh_response;
        vector<std::string> all_players;
        QList<QString> list = _players->keys();
        for(int i=0;i<_players->size();i++)
            all_players.push_back((list.at(i)).toStdString());

        int amount = irh.match(irh_response,temp_image,all_players);
		qDebug() << __FILE__ << __LINE__ << __func__ << amount; // DEBUG
        if(amount >= 0){
            QString victim = QString::fromStdString(irh_response);
            PlayerFactory::getPlayer(victim)->health -= amount;

            if(PlayerFactory::getPlayer(victim)->health <= 0) {
                PlayerFactory::getPlayer(victim)->_alive = false;
                if(server->db->addDeath(irh_response))
                    cout<<"add death from game to db works"<<endl;
                else
                    cout<<"add death from game to db does not work"<<endl;
                if(server->db->addKill(player.toStdString()))
                    cout<<"add kill from game to db works"<<endl;
                else
                    cout<<"add kill from game to db does not work"<<endl;

            }
            for(int i=0;i<_players->size();i++){
            PlayerFactory::getPlayer(_players->keys().at(i))->hit(player, victim, amount);
            }
            int aAlive=0, bAlive=0, aTotal=0, bTotal=0;
            QString tmpName;
            //bool tmpAlive;
            int tmpHealth;
            for(int i=0;i<_players->size();i++) {
                tmpName = _players->keys().at(i);
                tmpHealth = PlayerFactory::getPlayer(tmpName)->health;
                if(_players->value(tmpName).compare("teamA") == 0) {
                    aTotal++;
                    if (tmpHealth > 0)
                        aAlive++;
                }
                else {
                    bTotal++;
                    if(tmpHealth > 0)
                        bAlive++;
                }
            }
            if(PlayerFactory::getPlayer(victim)->health > 0)
                for(int i=0;i<_players->size();i++){
                PlayerFactory::getPlayer(_players->keys().at(i))->gameUpdate(aAlive, aTotal, bAlive, bTotal, player, victim,
                                PlayerFactory::getPlayer(victim)->health, true);
                }
            else
                for(int i=0;i<_players->size();i++){
                PlayerFactory::getPlayer(_players->keys().at(i))->gameUpdate(aAlive, aTotal, bAlive, bTotal, player, victim,
                                PlayerFactory::getPlayer(victim)->health, false);
                }
        } else {
            for(int i=0;i<_players->size();i++){
            PlayerFactory::getPlayer(_players->keys().at(i))->miss(player);
            }
        }
    }catch(errors_e *e){
        std::cout << "ERROR MESSAGE: " << __FUNCTION__ << " : ImageRecognitionHelper raised an error: " << e << '\n';
        for(int i=0;i<_players->size();i++){
        PlayerFactory::getPlayer(_players->keys().at(i))->miss(player);
        }
    }
}

QString game::getGameId(){
    return _game_id;
}

void game::onGameChange(){
    QHash<QString, int>* hash = _change_hash;
//	qDebug << *_change_hash;
    hash->clear();
	QList<QString> temp_list = _teams->keys();
	qDebug() << __FILE__ << " " << __LINE__ << " " << __func__ << " " << temp_list;
    for(QList<QString>::const_iterator i = temp_list.begin();i != temp_list.end(); i++){
		// crash at dereferencing i
//		qDebug() << *i;
//		qDebug() << this->playersInTeam(*i);
        hash->insert(*i,this->playersInTeam(*i));
    }
    for(int i=0;i<_players->size();i++){
    //PlayerFactory::getPlayer(_players->keys().at(i))->gameInfo(_game_id,getDuration(),hash);
    }
}

void game::onDelete(){
    delete _teams;
    delete _change_hash;
    delete _players;
    delete _timer;
    delete &_last_hit_player;
}

QString game::getWinningTeam(){
    QHash<QString, int> *hash= _change_hash;
    QList<QString> list = _players->values();
    for(QList<QString>::const_iterator i = list.begin(); i != list.end();i++){
        int val = hash->value((*i));
        hash->insert((*i), (val+ 1));
    }
    list = hash->keys();
    QString team("teamA");
    int alive = 0;
    for(QList<QString>::const_iterator i = list.begin();i != list.end();i++){
        int tmp = hash->value(*i);
        if(tmp > alive){
            alive = tmp;
            team = *i;
        }
    }
    return team;
}

QString game::getGameInfo(){
    QString str;
    str.append("GAMEINFO;").append(_game_id).append(";").append(QString("%1").arg(_duration));
    QList<QString> list = _teams->keys();
	Q_ASSERT(list.size() > 1);
	for(QList<QString>::const_iterator i = list.begin();i != list.end();i++){
		if (*i == "teamA") {
			str.append(QString(";%1/%2").arg(_teams->value(*i)).arg(teamAplayers));
		} else if (*i == "teamB") {
			str.append(QString(";%1/%2").arg(_teams->value(*i)).arg(teamBplayers));
		} else {
			str.append(QString(";%1/%2").arg(_teams->value(*i)).arg(_max_players/2));
		}
	}
    return str;
}
