#include "playerfactory.h"
namespace PlayerFactory{
    namespace{
        QHash<QString*, Player*> _players;
        QObject _root;
    }

    bool exists(QString *id){
        if(id == NULL){
            return false;
        }
        return _players.contains(id);
    }

    Player* getPlayer(QString *id){
        if(id == NULL){
            return NULL;
        }
        if(exists(id)){
            return _players.value(id);
        }else{
            Player* player = new Player(id,&_root);
            _players.insert(id,player);
            return player;
        }
    }

    void destroyPlayer(QString *id){
        if(id == NULL){
            return;
        }
        _players.remove(id);
    }
}
