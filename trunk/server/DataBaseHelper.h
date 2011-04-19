/*
 * DataBaseHelper.h
 *
 * Author: Karthik Mallavarapu
 */


#ifndef DATABASEHELPER_H_
#define DATABASEHELPER_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QString>
#include <QSqlQuery>

#include "PlayerInfo.h"
#include<stdint.h>
#include<sys/stat.h>

class DataBaseHelper {
public:
	DataBaseHelper();
	string getPassword(string);
	bool sendInvite(string,uint64_t);
	bool addKill(string);
	bool addDeath(string);
	vector<uint64_t> getInvites(string);
	bool removeInvite(string,uint64_t);
	bool addPlayer(string,string);
	bool removePlayer(string);
        bool setScore(string userId,int value);
        int getDeaths(string userId);
        int getScore(string userId);
        int getKills(string userId);
        bool readFromDataBase();
        bool writeToDataBase();
        bool openDataBase();
        bool insertValues(QString uid,QString password,int kills,int score,int deaths);
        void testfunction();

private:

	vector<PlayerInfo *> playerVector;
	virtual ~DataBaseHelper();
	vector<uint64_t> playerInvites;
        QSqlDatabase db;


};

#endif /* DATABASEHELPER_H_ */
