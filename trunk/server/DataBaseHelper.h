/*
 * DataBaseHelper.h
 *
 * Author: Karthik Mallavarapu
 */

#ifndef DATABASEHELPER_H_
#define DATABASEHELPER_H_

#include "PlayerInfo.h"
#include<stdint.h>
#include<sys/stat.h>

const string path = "/tmp/database.csv";
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
	vector<int> getStatistics(string);


private:

	vector<PlayerInfo *> playerVector;
	virtual ~DataBaseHelper();
	vector<uint64_t> playerInvites;
};

#endif /* DATABASEHELPER_H_ */
