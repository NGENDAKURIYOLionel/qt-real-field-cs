/*
 * PlayerInfo.h
 *
 * Author: Karthik Mallavarapu
 */

#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_

#include<string>
#include<cstdio>
#include<iostream>
#include<vector>
//#include<tr1/cstdint>

class PlayerInfo {
public:
	PlayerInfo(std::string,std::string);
	virtual ~PlayerInfo();
        void setScore(int);
	std::string getUID();
	std::string getPassword();
	void addKill();
	void addDeath();
	int getScore();
	int getDeaths();
	int getKills();
	bool addInvite(uint64_t);
	bool removeInvite(uint64_t);
	std::vector<uint64_t> getInvites();
	void printInvite();

private:
	std::string UID;
	std::string password;
	int numberOfDeaths;
	int numberOfKills;
	int numberOfInvites;
        int score;
	std::vector<uint64_t> invites;


};

#endif /* PLAYERINFO_H_ */
