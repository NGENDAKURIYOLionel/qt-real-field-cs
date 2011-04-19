#include "gamefactory.h"

GameFactory::GameFactory(QObject *parent) :
    QObject(parent)
{
    _games = new QHash<QString*, game*>();
}

bool GameFactory::exists(QString *id){
    if(id == NULL){
        return false;
    }
    return _games->contains(id);
}

game* GameFactory::getGame(QString* id){
    if(exists(id)){
        return _games->value(id);
    }else{
        game* new_game = new game(id, &_factory);
        _games->insert(id, new_game);
        return new_game;
    }
}

QList<QString*> GameFactory::getGameIds(){
    return _games->keys();
}

GameFactory* GameFactory::get(){
    return &_factory;
}

void GameFactory::destroyGame(QString* gameid){
    game* game = _games->value(gameid);
    game->deleteLater();
    _games->remove(gameid);
}
