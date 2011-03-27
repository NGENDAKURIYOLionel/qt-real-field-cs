#include <QtGui/QApplication>
#include <QDeclarativeItem>
#include <QDeclarativeContext>
#include <QDeclarativeProperty>
#include <QObject>
#include "qmlapplicationviewer.h"
#include "applicationdata.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;
    applicationData data;
    // set variable in qml exchange value between qml and c++
    view.rootContext()->setContextProperty("applicationData",  &data);
    view.setSource(QUrl::fromLocalFile("qml/test/webbrowser.qml"));
    view.show();
    QObject *object = qobject_cast<QObject*>(view.rootObject());

    QObject *text = object->findChild<QObject*>("text");
    if (text)
        QObject::connect(text, SIGNAL(qmlSignal(QString)),
                          &data, SLOT(cppSlot1(QString)));



    // change property
    qDebug() << "Property value:" << QDeclarativeProperty::read(object, "urlString");
    QDeclarativeProperty::write(object, "urlString", "http://www.hsl.fi");

    qDebug() << "Property value:" << object->property("urlString");
    object->setProperty("urlString", "http://www.cnn.com");

    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(object);
    item->setWidth(500);
    //change property of class variables
    //add objectName and call the name when assigning new value
    QObject *scrollBar = object->findChild<QObject*>("scrollBar");
    if (scrollBar)
        scrollBar->setProperty("width", 50);
    QObject *backButton = object->findChild<QObject*>("backButton");
    if (backButton)
        backButton->setProperty("image", "qml/test/content/pics/go-next-view.png");

    return app.exec();
}
