/*
 * DataBaseHelper.cpp
 *
 * Author: Karthik Mallavarapu
 */

#include "DataBaseHelper.h"
#include<QDir>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>
#include <string>
#include <vector>
using namespace std;

DataBaseHelper::DataBaseHelper() {
	// TODO Auto-generated constructor stub
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("test.db");
    if(!(QFile::exists("test.db"))) {
    if(db.open()) {
        QSqlQuery query;
        if(query.exec("create table Player " "(UID varchar(20) primary key, " "Password varchar(20), "
                      "Kills integer, " "Score integer, " "Deaths integer)"))
            cout<<"works"<<endl;
        else
            cout<<"dint work"<<endl;
    }
    }

    /*if(insertValues("test1","test1",11,11,11))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;
    if(insertValues("test2","test2",12,12,12))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;
    if(insertValues("test3","test3",13,13,13))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;
    if(insertValues("test4","test4",14,14,14))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;*/

    /*if(readFromDataBase())
        cout<<"true"<<endl;*/


}

bool DataBaseHelper::insertValues(QString UID,QString Password,int Kills,int Score,int Deaths) {

    if(db.open()) {
    QSqlQuery query(db);
    if(query.exec(QString("INSERT INTO Player Values ('%1','%2',%3,%4,%5)")
            .arg(UID).arg(Password).arg(Kills).arg(Score).arg(Deaths)))
        return true;
    else {
        qDebug() << query.lastError();
        return false;
    }
    }
}

bool DataBaseHelper::openDataBase() {

    db.setDatabaseName("test.db");
    if(db.open()) {
        QSqlQuery query;
        if(query.exec("create table Player " "(UID varchar(20) primary key, " "Password varchar(20), "
                      "Kills integer, " "Score integer, "
                      "Deaths integer)"))
            return true;
        else
            return false;
    }


}

bool DataBaseHelper::writeToDataBase() {
    if(db.open()) {
        QSqlQuery query(db);
        query.exec("SELECT * from Player");
        QSqlRecord rec = query.record();
        if(query.isSelect()) {
            while(query.next()) {

                QSqlQuery update(db);
                QString username = query.value(rec.indexOf("UID")).toString();
                int score = query.value(rec.indexOf("Score")).toInt() + getScore(username.toStdString());
                int kills = query.value(rec.indexOf("Kills")).toInt() + getKills(username.toStdString());
                int deaths = query.value(rec.indexOf("Deaths")).toInt() + getDeaths(username.toStdString());
                cout<<" "<<score<<" "<<kills<<" "<<deaths<<" "<<username.toStdString()<<endl;
                if(update.exec(QString("UPDATE Player SET Score=%2,Kills=%3,Deaths=%4 WHERE UID='%1' ").arg(username).arg(score).arg(kills).arg(deaths)))
                       cout<<"Updated database table successfully"<<endl;

            }
            return true;

        }

    }

    return true;
}

bool DataBaseHelper::readFromDataBase() {
    if(db.open()) {
        QSqlQuery query(db);
        query.exec("SELECT * from Player");
        QSqlRecord rec = query.record();
        if(query.isSelect()) {
            while(query.next()) {

                QString username = query.value(rec.indexOf("UID")).toString();
                QString password = query.value(rec.indexOf("Password")).toString();
                if(addPlayer(username.toStdString(),password.toStdString()))
                    cout<<"added player successfully"<<endl;

            }
            return true;

        }

    }

    else
        return false;
}

string DataBaseHelper::getPassword(string userId) {
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
		if((*it)->getUID() == userId) {
			return (*it)->getPassword();
		}

	}
        return NULL;
}

int DataBaseHelper::getScore(string userId) {
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        return (*it)->getScore();
                }

        }
        return NULL;
}

int DataBaseHelper::getDeaths(string userId) {
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        return (*it)->getDeaths();
                }

        }
        return NULL;
}

int DataBaseHelper::getKills(string userId) {
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        return (*it)->getKills();
                }

        }
        return NULL;
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
bool DataBaseHelper::setScore(string userId,int value) {
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {

                                if((*it)->getUID() == userId) {
                                        (*it)->setScore(value);
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

void DataBaseHelper::testfunction() {
    if(readFromDataBase()) {
        if(setScore("test1",10))
            cout<<"Set Score Success"<<endl;
        if(setScore("test2",10))
            cout<<"Set Score Success"<<endl;
        if(setScore("test3",10))
            cout<<"Set Score Success"<<endl;
        if(setScore("test4",10))
            cout<<"Set Score Success"<<endl;
        if(addDeath("test1"))
            cout<<"Add Death Success"<<endl;
        if(addDeath("test2"))
            cout<<"Add Death Success"<<endl;
        if(addDeath("test3"))
            cout<<"Add Death Success"<<endl;
        if(addDeath("test4"))
            cout<<"Add Death Success"<<endl;
        if(addKill("test1"))
            cout<<"Add Kill Success"<<endl;
        if(addKill("test2"))
            cout<<"Add Kill Success"<<endl;
        if(addKill("test3"))
            cout<<"Add Kill Success"<<endl;
        if(addKill("test4"))
            cout<<"Add Kill Success"<<endl;
    }
    if(writeToDataBase())
        cout<<"Written to database"<<endl;



}

DataBaseHelper::~DataBaseHelper() {
	// TODO Auto-generated destructor stub
    delete &db;
    playerVector.clear();
    playerInvites.clear();

}
