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
                      "Kills integer, " "Score integer, " "Deaths integer," "imagePath varchar(100))")) {
                   cout<<"works"<<endl;
                   insertDummy();
        }
        else
            cout<<"dint work"<<endl;
        }
    }
    else {
          QSqlQuery query;
          if(query.exec("select count(*) from Player") == 0)
              insertDummy();
    }

    if(readFromDataBase())
        cout<<"read database works"<<endl;
    else
        cout<<"did not really work"<<endl;
}

void DataBaseHelper::insertDummy() {
    if(insertValues("karthik","123",0,0,0,"images.jpeg"))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;

    if(insertValues("risto","1234",0,0,0,"images.jpeg"))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;
    /*if(insertValues("test3","test3",0,0,0,"images.jpeg"))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;
    if(insertValues("test4","test4",0,0,0,"images.jpeg"))
        cout<<"method works"<<endl;
    else
         cout<<"method dint work"<<endl;*/
}

bool DataBaseHelper::insertValues(QString UID,QString Password,int Kills,int Score,int Deaths,QString imagePath) {

    if(db.open()) {
    QSqlQuery query(db);
    if(query.exec(QString("INSERT INTO Player Values ('%1','%2',%3,%4,%5,'%6')")
            .arg(UID).arg(Password).arg(Kills).arg(Score).arg(Deaths).arg(imagePath)))
        return true;
    else {
        qDebug() << query.lastError();
        return false;
    }
    }
    else
        return false;
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
                else
                    qDebug()<<update.lastError();

            }
            return true;

        }

    }

    return false;
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
                QString imagePath = query.value(rec.indexOf("imagePath")).toString();
                if(addPlayer(username.toStdString(),password.toStdString(),imagePath.toStdString()))
                    cout<<"added player successfully"<<endl;

            }
            return true;

        }
        else
            return false;

    }

    else
        return false;
}

string DataBaseHelper::getPassword(string userId) {
        string password;
	for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
		if((*it)->getUID() == userId) {
                        password = (*it)->getPassword();
                        //return (*it)->getPassword();
		}

	}
        return password;
}
/*Returns score of a player if getScore() is a success. Else returns -1*/
int DataBaseHelper::getScore(string userId) {
        int score = -1;
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        score = (*it)->getScore();
                }

        }
        return score;
}

/*Returns number of deaths of a player if getDeaths() is a success. Else returns -1*/
int DataBaseHelper::getDeaths(string userId) {
        int deaths = -1;
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        deaths = (*it)->getDeaths();
                }

        }
        return deaths;
}
/*Returns number of kills of a player if getKills() is a success. Else returns -1*/
int DataBaseHelper::getKills(string userId) {
        int kills = -1;
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
                if((*it)->getUID() == userId) {
                        kills = (*it)->getKills();
                }

        }
        return kills;
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

void DataBaseHelper::getPlayers(vector<string> *players) {
    for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        players->push_back((*it)->getUID());
    }

}

string DataBaseHelper::getImagePath(string userId) {
    string imagePath;
    for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        if((*it)->getUID() == userId) {
            imagePath = (*it)->getImagePath();
        }

    }
    return imagePath;
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

bool DataBaseHelper::addPlayer(string userId,string password,string imagePath) {

    if((!userId.empty()) && (!password.empty())) {
        for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
            if((*it)->getUID() == userId)
            return false;
        }
        PlayerInfo *player = new PlayerInfo(userId,password,imagePath);
        playerVector.push_back(player);
        return true;
    }
    else
        return false;
}

bool DataBaseHelper::removePlayer(string userId) {

    for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        if((*it)->getUID() == userId) {
            playerVector.erase(it);
            return true;
        }

    }
    return false;
}

bool DataBaseHelper::sendInvite(string userId,uint64_t gameId) {
    for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        if((*it)->getUID() == userId) {
            if((*it)->addInvite(gameId))
                return true;
        }
    }
    return false;
}

bool DataBaseHelper::removeInvite(string userId,uint64_t gameId) {
    for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        if((*it)->getUID() == userId) {
            if((*it)->removeInvite(gameId))
                return true;
        }
    }
    return false;
}

void DataBaseHelper::getInvites(string userId,vector<uint64_t> *playerInvites) {
   for(vector<PlayerInfo *>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        if((*it)->getUID() == userId) {
            vector<uint64_t> temp = (*it)->getInvites();
            for(vector<uint64_t>::iterator i = temp.begin();i < temp.end();i++)
                playerInvites->push_back((*i));
            break;
        }
    }

}

void DataBaseHelper::testfunction() {
    /*if(readFromDataBase()) {

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

    }*/
    if(!(getImagePath("test1").empty()))
        cout<<"Works"<<endl;
    if(!(getImagePath("test5").empty()))
        cout<<"Works"<<endl;
    else
        cout<<"NULL string test works"<<endl;

    /*if(writeToDataBase())
        cout<<"Written to database"<<endl;*/
}

DataBaseHelper::~DataBaseHelper() {
	// TODO Auto-generated destructor stub
    //delete &db;
    /*for(vector<PlayerInfo*>::iterator it = playerVector.begin();it < playerVector.end();it++) {
        delete *it;
    }*/
    //delete &playerInvites;

}
