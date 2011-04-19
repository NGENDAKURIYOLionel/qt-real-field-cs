#include "playerfactory.h"

PlayerFactory::PlayerFactory(QObject *parent) :
    QObject(parent)
{
    _players = new QHash<QString*, PlayerInfo*>();
}

PlayerFactory* PlayerFactory::get(){
    return &_factory;
}

bool PlayerFactory::exists(QString *id){
    if(id == NULL){
        return false;
    }
    return _players->contains(id);
}

PlayerInfo* PlayerFactory::getPlayer(QString *id){
    if(exists(id)){
        return _players->value(id);
    }else{
        //TODO create new player
        return NULL;
    }
}
