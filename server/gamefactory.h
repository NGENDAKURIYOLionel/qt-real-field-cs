#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <QObject>
#include <QList>
#include <QHash>
#include "game.h"
#include "server.h"
#include "player.h"
#include "playerfactory.h"

class game;
class Server;
namespace GameFactory{
    bool exists(QString id);
    game* getGame(QString id);
    QList<QString> getGameIds();
    void destroyGame(QString id);
    void setServer(Server *s);
    void updateGameList();
}

#endif // GAMEFACTORY_H
