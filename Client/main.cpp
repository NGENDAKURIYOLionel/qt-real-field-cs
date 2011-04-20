#include <QApplication>
#include "client.h"
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Client client;
//#ifdef Q_OS_SYMBIAN
//    // Make application better looking and more usable on small screen
//    client.showMaximized();
//#else
//    client.show();
//#endif

    const QString mainQmlApp = QLatin1String("qrc:/client.qml");
    QDeclarativeView view;
    view.engine()->addImportPath(QString("/opt/qtm11/imports"));
#if !defined(QT_NO_OPENGL) && !defined(Q_WS_MAEMO_5) && !defined(Q_WS_MAEMO_6) && !defined(Q_WS_S60)
    view.setViewport(new QGLWidget);
#endif
    view.setSource(QUrl(mainQmlApp));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view.rootContext()->setContextProperty("client",new Client);
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    view.showFullScreen();
#else
    view.setGeometry(QRect(100, 100, 800, 480));
    view.show();
#endif


    return app.exec();
}
