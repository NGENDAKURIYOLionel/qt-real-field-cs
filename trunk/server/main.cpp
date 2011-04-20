#include <cstdlib>
#include <iostream>
#include <QApplication>
#include <QtCore>
#include "server.h"
#include "imagerecognitionhelper.h"

#define NAMESPACE_NAME "20110419_2"
std::string current_namespace(NAMESPACE_NAME);
ImageRecognitionHelper irh(current_namespace);

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
