#include <QtGui>
#include <QtNetwork>

#include "client.h"

QGridLayout *mainLayout;
QWidget *Additional;
QWidget *p;
Client::Client(QWidget *parent)
:   QDialog(parent), networkSession(0)
{
    hostLabel = new QLabel(tr("&Server name:"));
    portLabel = new QLabel(tr("S&erver port:"));
    //p=parent;
    // find out which IP to connect to
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    hostLineEdit = new QLineEdit(ipAddress);
    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    hostLabel->setBuddy(hostLineEdit);
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("Waiting for connection"));

    connectButton = new QPushButton(tr("connect"));
    connectButton->setDefault(true);
    connectButton->setEnabled(true);

    sendLineButton = new QPushButton(tr("send line"));
    sendLineButton->setDefault(true);
    sendLineButton->setEnabled(false);

    readLineButton = new QPushButton(tr("read line"));
    readLineButton->setDefault(true);
    readLineButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(readLineButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(sendLineButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    tcpSocket = new QTcpSocket(this);

//    connect(hostLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enablereadLineButton()));
//    connect(portLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enablereadLineButton()));
//    connect(hostLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enablesendLineButton()));
//    connect(portLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enablesendLineButton()));
    connect(readLineButton, SIGNAL(clicked()),
            this, SLOT(readLine()));
    connect(sendLineButton, SIGNAL(clicked()),
            this, SLOT(sendLine()));
    connect(connectButton, SIGNAL(clicked()),
            this, SLOT(connectto()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(enablereadLineButton()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(enablesendLineButton()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Test Client"));
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        readLineButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

void Client::connectto()
{
    readLineButton->setEnabled(false);
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(hostLineEdit->text(),
                             portLineEdit->text().toInt());
}

void Client::connected()
{
    statusLabel->setText("connected");
}

void Client::readLine()
{
    QTextStream in(tcpSocket);

    QString newMessage;
    newMessage=in.readAll();
    readLineButton->setEnabled(false);

    currentMessage = newMessage;
    statusLabel->setText(currentMessage);
}

void Client::sendLine()
{
    sendMessage(QString("change"));
}

void Client::sendMessage(QString message){
    tcpSocket->write(message.toAscii());
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    readLineButton->setEnabled(true);
}

void Client::enableconnectButton()
{
    connectButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostLineEdit->text().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::enablereadLineButton()
{
    //readLineButton->setEnabled(networkSession->isOpen());
    readLineButton->setEnabled(!networkSession || networkSession->isOpen());
}

void Client::enablesendLineButton()
{
    sendLineButton->setEnabled(!networkSession || networkSession->isOpen());
}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    statusLabel->setText(tr("This examples requires that you run the "
                            "Fortune Server example as well."));

    enablereadLineButton();
}
