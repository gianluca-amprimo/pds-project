#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "client.h"
#include "ui_login.h"
#include <iostream>
#include <string>
//! [0]
Client::Client(QWidget *parent)
        : QDialog(parent),tcpSocket(new QTcpSocket(this)),ui(new Ui::Client)

{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
//! [0]
    ui->logButton->setDefault(true);
    ui->logButton->setEnabled(false);

//! [1]
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

//! [1]

    connect(ui->passwordLE, &QLineEdit::textChanged, this, &Client::enableLogButton);
    connect(ui->logButton, &QAbstractButton::clicked,this, &Client::requestLogin);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readResponse);
    connect(tcpSocket, &QAbstractSocket::connected  ,this, &Client::sendCredentials);
//! [2] //! [4]
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
//! [3]
            this, &Client::displayError);
//! [4]


    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("PdsProject"));
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
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        ui->logButton->setEnabled(false);
        ui->ResultTE->setText(tr("Opening network session."));
        networkSession->open();
    }
//! [5]
}
//! [5]

//! [6]
void Client::requestLogin()
{
    auto host="192.168.1.86";
    int port=36107;
    ui->logButton->setEnabled(false);
    tcpSocket->abort();
//! [7]
    tcpSocket->connectToHost(host,
                             port);
//! [7]
    ui->ResultTE->setText("Trying to connect...");

}
//! [6]

//! [8]
void Client::readResponse()
{
    ui->ResultTE->append("Reading the response");
    in.startTransaction();

    QString response;
    in >> response;

    if (!in.commitTransaction())
        return;

    ui->ResultTE->append(response);
    ui->logButton->setEnabled(true);
}
//! [8]

//! [13]
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

   ui->logButton->setEnabled(true);
}
//! [13]

void Client::enableLogButton()
{
    ui->logButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !ui->userLE->text().isEmpty() &&
                                 !ui->passwordLE->text().isEmpty());

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

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    enableLogButton();
}

void Client::sendCredentials(){

    ui->ResultTE->append("Connected to server");

    if (tcpSocket != nullptr) {
        if (!tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        out << QString(ui->userLE->text() + "_" + ui->passwordLE->text());
        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("Server"), tr("Could not send message"));
        }
        tcpSocket->flush();
    }

    ui->ResultTE->append("Credential sent, waiting for reply...");

}

