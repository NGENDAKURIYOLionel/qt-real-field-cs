#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <QtCore>
#include "server.h"

int main(int argc, char *argv[])
{
	Server server;
	QApplication app(argc, argv);
	if (server.listen(QHostAddress("LocalHost")), 59999) {
		std::cout << "port:" << server.serverPort() <<"\n";
	}
	else {
		std::cout << "server port is unkown\n";
	}

	return app.exec();
}
