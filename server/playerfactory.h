#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include <QObject>
#include <QHash>
#include "PlayerInfo.h"

namespace PlayerFactory{
    static bool exists(QString* id);
    static PlayerInfo* getPlayer(QString* id);
}

#endif // PLAYERFACTORY_H
