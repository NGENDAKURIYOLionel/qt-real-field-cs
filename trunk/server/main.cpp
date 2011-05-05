#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <QApplication>
#include <QtCore>
#include "server.h"
#include "imagerecognitionhelper.h"
#include "DataBaseHelper.h"
#include "cstdio"
#include "player.h"
#include "playerfactory.h"

std::string current_namespace(NAMESPACE_NAME);
ImageRecognitionHelper irh(current_namespace);

// FIXME: needs to be deallocated somewhere?
//DataBaseHelper* db_global = new DataBaseHelper();

typedef enum {
	FILE_NOT_FOUND
} main_errors;

static void read_file(const char* src, std::string& target) {
	FILE* f = fopen(src, "rb");
	if (f == NULL) throw FILE_NOT_FOUND;
#define BUFSIZE 4096
	unsigned char buf[BUFSIZE];
	while(1) {
		size_t read = fread(buf, 1, BUFSIZE, f);
		if (read > 0) target.append((const char*)buf, read);
		if (feof(f)) break;
	}
	fclose(f);
}

static void register_player(std::string& uid, std::string& picture) {
	std::cout << "registering user " << uid << std::endl;
	std::string jpeg_picture_data;
	try {
		read_file(picture.c_str(), jpeg_picture_data);
	} catch(...) {
		std::cout << "error reading image" << std::endl;
		return;
	}
	try {
		irh.register_player(uid, jpeg_picture_data);
	} catch(errors_e e) {
		switch (e) {
			case IRH_ERROR_CURL: std::cout << "curl error" << std::endl; break;
			case IRH_ERROR_JSON: std::cout << "json error" << std::endl; break;
			case IRH_ERROR_FACE_DOT_COM: std::cout << "face.com error" << std::endl; break;
			case IRH_ERROR_PHOTO_HAS_NO_FACES: std::cout << "no face" << std::endl; break;
			case IRH_ERROR_PHOTO_HAS_NO_RECOGNIZABLE_FACE: std::cout << "no recog. face" << std::endl; break;
			default: std::cout << "other error" << std::endl; break;
		}
		std::cout << "error registering " << uid << std::endl;
	}
}

int main(int argc, char *argv[])
//int dummy(int argc, char *argv[])
{
	try {
		std::cout << "contacting face.com" << std::endl;
		std::vector<std::string> users;
		irh.account_users(users);
	} catch (...) {
		std::cout << "error using face.com - is your namespace set correctly?" << std::endl;
		return 1;
	}
	std::cout << "contacted face.com" << std::endl;

//	std::vector<std::string> db_players;
//	db_global->getPlayers(&db_players);
//	if (db_players.size() == 0) {
//		std::cout << "please register players before starting the server" << std::endl;
//		return 1;
//	}
//	for (unsigned i = 0; i < db_players.size(); i++) {
//		std::string temp_image(db_global->getImagePath(db_players[i]));
//		register_player(db_players[i], temp_image);
//	}

	Server server;
        PlayerFactory::setServer(&server);

        cout<<server.db->getPassword("test1")<<endl;
        /*QString uid = "test1";
        QString passwd = "test1";
        Player *p = new Player(uid,0,&server);
        p->loginWithPassword(uid,passwd);*/
	QApplication app(argc, argv);
	if (server.listen(QHostAddress::Any,8888)) {
		std::cout << "server listening at port:" << server.serverPort() << std::endl;
	}
	else {
		std::cout << "unable to start server" << std::endl;
		return 1;
	}

	return app.exec();
}
