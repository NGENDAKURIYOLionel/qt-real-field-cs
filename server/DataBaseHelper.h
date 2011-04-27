/*
 * DataBaseHelper.h
 *
 * Author: Karthik Mallavarapu
 * Install required libraries: apt-get install libqt4-sql-sqlite
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

#include<sys/stat.h>
#include <string>
#include <vector>

class DataBaseHelper {
public:
	DataBaseHelper();
        ~DataBaseHelper();
	std::string getPassword(std::string);
	bool sendInvite(std::string,uint64_t);
	bool addKill(std::string);
	bool addDeath(std::string);
        void getInvites(std::string,vector<uint64_t> *);
	bool removeInvite(std::string,uint64_t);
        bool addPlayer(std::string,std::string,std::string);
	bool removePlayer(std::string);
        bool setScore(std::string userId,int value);
        int getDeaths(std::string userId);
        int getScore(std::string userId);
        int getKills(std::string userId);
        bool readFromDataBase();
        bool writeToDataBase();
        bool openDataBase();
        bool insertValues(QString uid,QString password,int kills,int score,int deaths,QString imagePath);
        void testfunction();
        void getPlayers(vector<string> *);
        string getImagePath(string);


private:

	std::vector<PlayerInfo *> playerVector;
        QSqlDatabase db;


};

#endif /* DATABASEHELPER_H_ */
