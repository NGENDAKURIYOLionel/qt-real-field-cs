#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <QObject>
#include <QList>
#include <QHash>
#include "game.h"

class GameFactory : public QObject
{
    Q_OBJECT
public:
    static GameFactory* get();
    static bool exists(QString* id);
    static game* getGame(QString* id);
    static QList<QString*> getGameIds();
    static void destroyGame(QString* id);
signals:

public slots:

private:
    explicit GameFactory(QObject *parent = 0);
    static QHash<QString*, game*> *_games;
    static GameFactory _factory;
};

#endif // GAMEFACTORY_H
