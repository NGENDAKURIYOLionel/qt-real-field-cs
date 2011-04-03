/*
 * DataBaseHelper.cpp
 *
 * Author: Karthik Mallavarapu
 */

#include "DataBaseHelper.h"

DataBaseHelper::DataBaseHelper() {
	// TODO Auto-generated constructor stub

/*    struct stat stFileInfo;
	if(stat(path.c_str(),&stFileInfo) == 0) {

		//file exists. read from CSV file.
		std::cout << "File exists"<<std::endl;
	}
	else
		std::cout << "File does not exist"<<std::endl;*/

}
string DataBaseHelper::getPassword(string userId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
		if((*it)->getUID() == userId) {
			return (*it)->getPassword();
		}

	}
}

bool DataBaseHelper::addKill(string userId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
			if((*it)->getUID() == userId) {
				(*it)->addKill();
				return true;
			}

		}
	return false;

}

bool DataBaseHelper::addDeath(string userId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {

				if((*it)->getUID() == userId) {
					(*it)->addDeath();
					return true;
				}

			}
		return false;

}

bool DataBaseHelper::addPlayer(string userId,string password) {

	if((!userId.empty()) && (!password.empty())) {
		for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
			if((*it)->getUID() == userId)
				return false;
		}
		PlayerInfo *player = new PlayerInfo(userId,password);
		playerVector.push_back(player);
		return true;
	}
	else
		return false;
}

bool DataBaseHelper::removePlayer(string userId) {

	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
		if((*it)->getUID() == userId)
		{
			playerVector.erase(it);
			return true;
		}

	}
	return false;
}

vector<int> DataBaseHelper::getStatistics(string userId) {
	vector<int> test;
	return test;

}

bool DataBaseHelper::sendInvite(string userId,uint64_t gameId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
			if((*it)->getUID() == userId)
			{
				if((*it)->addInvite(gameId))
					return true;
			}
	}
	return false;
}

bool DataBaseHelper::removeInvite(string userId,uint64_t gameId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
				if((*it)->getUID() == userId)
				{
					if((*it)->removeInvite(gameId))
						return true;
				}
		}
		return false;

}

vector<uint64_t> DataBaseHelper::getInvites(string userId) {
	playerInvites.clear();
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
			if((*it)->getUID() == userId)
			{
				playerInvites = (*it)->getInvites();
				break;
			}

		}
	return playerInvites;
}


DataBaseHelper::~DataBaseHelper() {
	// TODO Auto-generated destructor stub
}
