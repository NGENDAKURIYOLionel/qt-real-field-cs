#include "playerfactory.h"
namespace PlayerFactory{
    namespace{
        QHash<QString*, PlayerInfo*> _players;
    }

    bool exists(QString *id){
        if(id == NULL){
            return false;
        }
        return _players.contains(id);
    }

    PlayerInfo* getPlayer(QString *id){
        if(exists(id)){
            return _players.value(id);
        }else{
            //TODO create new player
            return NULL;
        }
    }
}
