#include "playerfactory.h"
namespace PlayerFactory{
    namespace{
        QHash<QString, Player*> _players;
        QObject _root;
        Server *server;
    }

    void setServer(Server *s) {
        server = s;
    }

    bool exists(QString id){
        if(id == NULL){
            return false;
        }
        return _players.contains(id);
    }

    Player* getPlayer(QString id){
        if(id == NULL){
            return NULL;
        }
        if(exists(id)){
            return _players.value(id);
        }else{
            Player* player = new Player(id,0,server);
            _players.insert(id,player);
            return player;
        }
    }

    void destroyPlayer(QString id, MessageHandler *handler){
        if(id == NULL){
            return;
        }
        Player *player = _players.value(id);
        if(player->inGame() == false){
            player->disconnect(handler);
            handler->disconnect(player);
            _players.remove(id);
           delete player;
        }
    }

    QList<Player*> getPlayers(){
        return _players.values();
    }
}
