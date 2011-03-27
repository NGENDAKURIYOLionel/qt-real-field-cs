#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <QDateTime>
#include <QObject>
#include <QDebug>

class applicationData : public QObject
{
    Q_OBJECT
public:
    explicit applicationData(QObject *parent = 0);
    Q_INVOKABLE QDateTime getCurrentDateTime() const {
        return QDateTime::currentDateTime();
    }
    Q_INVOKABLE void cppMethod(const QString &msg) {
            qDebug() << "Called the C++ method with" << msg;
        }
signals:

public slots:
    void cppSlot(int number) {
            qDebug() << "Called the C++ slot with" << number;
        }

    void cppSlot1(const QString &msg) {
           qDebug() << "Called the C++ slot with message:" << msg;
       }
};

#endif // APPLICATIONDATA_H
