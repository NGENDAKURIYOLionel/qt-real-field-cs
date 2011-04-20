#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <QObject>
#include <QList>
#include <QHash>
#include "game.h"

namespace GameFactory{
    bool exists(QString* id);
    game* getGame(QString* id);
    QList<QString*> getGameIds();
    void destroyGame(QString* id);
}

#endif // GAMEFACTORY_H
