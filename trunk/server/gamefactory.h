#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <QObject>
#include <QList>
#include <QHash>
#include "game.h"
#include "server.h"

class game;
class Server;
namespace GameFactory{
    bool exists(QString id);
    game* getGame(QString id);
    QList<QString> getGameIds();
    void destroyGame(QString id);
    void setServer(Server *s);
}

#endif // GAMEFACTORY_H
