#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include <QObject>
#include <QHash>
#include "player.h"
#include "server.h"
#include "messagehandler.h"

class Player;

namespace PlayerFactory{
    bool exists(QString id);
    Player* getPlayer(QString id);
    void destroyPlayer(QString id, MessageHandler *handler);
    //Server *server;
    void setServer(Server *s);
    QList<Player*> getPlayers();

}

#endif // PLAYERFACTORY_H
