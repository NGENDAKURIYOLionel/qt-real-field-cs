#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <QtCore>
#include "server.h"

int main(int argc, char *argv[])
//int dummy(int argc, char *argv[])
{
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
