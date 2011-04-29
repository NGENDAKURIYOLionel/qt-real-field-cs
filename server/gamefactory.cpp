#include "gamefactory.h"
namespace GameFactory{
    namespace{
        QHash<QString*, game*> _games;
        QObject _root;
    }

    bool exists(QString *id){
        if(id == NULL){
            return false;
        }
        return _games.contains(id);
    }

    game* getGame(QString* id){
        if(id == NULL){
            return NULL;
        }
        if(exists(id)){
            return _games.value(id);
        }else{
            game* new_game = new game(id);
            _games.insert(id, new_game);
            return new_game;
        }
    }

    QList<QString*> getGameIds(){
        return _games.keys();
    }

    void destroyGame(QString* gameid){
        if(gameid == NULL){
            return;
        }
        game* game = _games.value(gameid);
        game->deleteLater();
        _games.remove(gameid);
    }
}
