#include "gamefactory.h"
namespace GameFactory{
    namespace{
        QHash<QString, game*> _games;
        QObject _root;
        Server *server;
    }

    bool exists(QString id){
        if(id == NULL){
            return false;
        }
        return _games.contains(id);
    }

    void setServer(Server *s) {
        server = s;
    }

    void updateGameList(){
        QList<QString> games=getGameIds();
        QString message("GAMELIST;");
        for(int i=0;i<games.size();i++){
            message.append(games.at(i));
            message.append(";");
        }
        QList<Player*> players=PlayerFactory::getPlayers();
        for(int i=0;i<players.size();i++){
            if(players.at(i)->inGame()==false){
                players.at(i)->updateGameList(message);
            }
        }
    }

    game* getGame(QString id){
        if(id == NULL){
            return NULL;
        }
        if(exists(id)){
            return _games.value(id);
        }else{
            game* new_game = new game(id,0,server);
            _games.insert(id, new_game);
            return new_game;

        }
    }

    QList<QString> getGameIds(){
        return _games.keys();
    }

    void destroyGame(QString gameid){
        if(gameid == NULL){
            return;
        }
        if (exists(gameid)){
        game* game = _games.value(gameid);
        game->deleteLater();
        _games.remove(gameid);
        updateGameList();
        }
    }


}
