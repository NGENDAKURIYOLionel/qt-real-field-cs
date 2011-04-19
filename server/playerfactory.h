#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include <QObject>
#include <QHash>
#include "PlayerInfo.h"

class PlayerFactory : public QObject
{
    Q_OBJECT
public:
    explicit PlayerFactory(QObject *parent = 0);
    static PlayerFactory *get();
    static bool exists(QString* id);
    static PlayerInfo* getPlayer(QString* id);
signals:

public slots:

private:
    static QHash<QString*, PlayerInfo*> *_players;
    static PlayerFactory _factory;
};

#endif // PLAYERFACTORY_H
