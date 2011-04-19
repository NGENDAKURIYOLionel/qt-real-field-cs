/*
 * PlayerInfo.cpp
 *
 * Author: Karthik Mallavarapu
 */

#include "PlayerInfo.h"
//#include<algorithm>
using namespace std;


PlayerInfo::PlayerInfo(string uid,string passwd) {
	// TODO Auto-generated constructor stub
	score = 0;
	numberOfDeaths = 0;
	numberOfKills = 0;
	UID = uid;
	password = passwd;
	numberOfInvites = 0;
	//invites = 0;
}

void PlayerInfo::setScore(int value) {
	score +=value;

}

string PlayerInfo::getUID() {
	return UID;
}

string PlayerInfo::getPassword() {
	return password;
}

void PlayerInfo::addDeath() {
	numberOfDeaths++;
}

void PlayerInfo::addKill() {
	numberOfKills++;
}

int PlayerInfo::getKills() {
	return numberOfKills;
}

int PlayerInfo::getDeaths() {
	return numberOfDeaths;
}

int PlayerInfo::getScore() {
	return score;
}

bool PlayerInfo::addInvite(uint64_t gameId) {

	for(vector<uint64_t>::iterator it = invites.begin();it < invites.end();it++) {
	if((*it) == gameId)
		return false;
	}
	numberOfInvites++;
	invites.push_back(gameId);
	return true;


}

bool PlayerInfo::removeInvite(uint64_t gameId) {
	for(vector<uint64_t>::iterator it = invites.begin();it < invites.end();it++) {
		if((*it) == gameId) {
			numberOfInvites--;
			invites.erase(it);
			return true;
			}
		}
	return false;
}

vector<uint64_t> PlayerInfo::getInvites() {
	return invites;

}

void PlayerInfo::printInvite() {
	cout<<"Printing the invite list"<<endl;
	for(vector<uint64_t>::iterator it = invites.begin();it < invites.end();it++)
		cout<<(*it)<<endl;

}

PlayerInfo::~PlayerInfo() {
	// TODO Auto-generated destructor stub
}
