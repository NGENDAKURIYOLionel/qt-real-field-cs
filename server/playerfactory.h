#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include <QObject>
#include <QHash>
#include "player.h"

namespace PlayerFactory{
    bool exists(QString* id);
    Player* getPlayer(QString* id);
    void destroyPlayer(QString* id);
}

#endif // PLAYERFACTORY_H
