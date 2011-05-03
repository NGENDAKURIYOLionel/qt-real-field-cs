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
    _timer->setSingleShot(true);
    _game_id = id;
    connect(_timer,SIGNAL(timeout()), this, SLOT(endGame()));
    connect(this,SIGNAL(joined(QString,QString,int,int)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(left(QString,QString,QString,int,int)),this,SLOT(onGameChange()));
    connect(this, SIGNAL(destroyed()),this,SLOT(onDelete()));
    server = s;
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
    _timer->setInterval(duration * 1000); //duration in milliseconds
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
        _players->insert(player,team);
        _teams->insert(team, _teams->value(team) + 1);
        qDebug() << "game emitting joined";
        emit joined(player,team, _teams->value("teamA"), _teams->value("teamB"));
    }
}
/*
 Leave the game
 */
void game::leaveGame(QString id){
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

        emit left(id,team, _game_id, aTotal, bTotal);
    }
}

/*
 Return the number of players in the given team or -1 if no such team
 */
int game::playersInTeam(QString team){
    if(_teams->contains(team)){
        return -1;
    }
    //get the values as a list from the QHash
    QList<QString> list = _players->values();
    int size = list.size(), count = 0;
    //go through the values
    for(int i = 0; i < size; i++){
        //if the team index is the same up the count
        if(list.at(i).compare(team)){
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
    QString win_team = getWinningTeam();
    QList<QString> players = _players->keys();
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

void game::shot(QByteArray* image, QString player){
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
            emit hit(player, victim, amount);
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
                emit gameUpdate(aAlive, aTotal, bAlive, bTotal, player, victim,
                                PlayerFactory::getPlayer(victim)->health, true);
            else
                emit gameUpdate(aAlive, aTotal, bAlive, bTotal, player, victim,
                                PlayerFactory::getPlayer(victim)->health, false);
        } else {
            emit miss(player);
        }
    }catch(errors_e *e){
        std::cout << "ERROR MESSAGE: " << __FUNCTION__ << " : ImageRecognitionHelper raised an error: " << e << '\n';
        emit miss(player);
    }
}

QString game::getGameId(){
    return _game_id;
}

void game::onGameChange(){
    QHash<QString, int> hash = _change_hash;
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
    emit gameInfo(_game_id,getDuration(),hash);
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
    for(QList<QString>::const_iterator i = list.begin();i != list.end();i++){
        str.append(QString("%1").arg(_teams->value(*i)));
    }
    return str;
}
