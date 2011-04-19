/*
 * PlayerInfo.h
 *
 * Author: Karthik Mallavarapu
 */

#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_

#include<string>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<stdint.h>
using namespace std;
class PlayerInfo {
public:
	PlayerInfo(string,string);
	virtual ~PlayerInfo();
        void setScore(int);
	string getUID();
	string getPassword();
	void addKill();
	void addDeath();
	int getScore();
	int getDeaths();
	int getKills();
	bool addInvite(uint64_t);
	bool removeInvite(uint64_t);
	vector<uint64_t> getInvites();
	void printInvite();

private:
	string UID;
	string password;
	int numberOfDeaths;
	int numberOfKills;
	int numberOfInvites;
        int score;
	vector<uint64_t> invites;


};

#endif /* PLAYERINFO_H_ */
