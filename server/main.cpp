#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <QApplication>
#include <QtCore>
#include "server.h"
#include "imagerecognitionhelper.h"
#include "DataBaseHelper.h"

#define NAMESPACE_NAME "20110419_2"
std::string current_namespace(NAMESPACE_NAME);
ImageRecognitionHelper irh(current_namespace);
DataBaseHelper db();


int main(int argc, char *argv[])
//int dummy(int argc, char *argv[])
{
	// TODO: register players
//	std::vector<std::string> db_players;
//	db.getPlayers(db_players);
//	if (db_players.size() == 0) {
//		std::cout << "please register players before starting the server" << std::endl;
//		return 1;
//	}
//	for (unsigned i = 0; i < db_players.size(); i++) {
////		db.getImage(db_players[i]);
//	}

	Server server;
	QApplication app(argc, argv);
	if (server.listen(QHostAddress("Any"),59999)) {
		std::cout << "server listening at port:" << server.serverPort() << std::endl;
	}
	else {
		std::cout << "unable to start server" << std::endl;
		return 1;
	}

	return app.exec();
}
