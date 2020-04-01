#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <iostream>
#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) : QDialog(parent), ui(new Ui::Server) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        /* Get saved network configuration.
         *
         * QSettings is an abstraction around application settings
         * (window sizes and positions, options, etc.) across sessions,
         * enabling to save and restore application settings in a portable manner.
         * When creating a QSettings object,
         * you must pass the name of your company or organization as well as the name of your application.
         */
        QSettings settings(QSettings::UserScope, QLatin1String("PiDiEsse"));
        // Groups are useful to avoid typing in the same setting paths over and over
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        ui->stato->setText(tr("Apro la sessione di rete ..."));
        networkSession->open();
    } else {
        sessionOpened();
    }
    // connect(tcpServer, &QTcpServer::newConnection, this, CALLBACK FOR NEW CONNECTION);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::getConnectedSocket);
    connect(ui->esci, &QAbstractButton::clicked, this, &QWidget::close);


    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sessionOpened() {
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("PiDiEsse"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("PiDiEsse [server]"),
                              tr("Impossibile avviare il server: %1.")
                                      .arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const auto & i : ipAddressesList) {
        if (i != QHostAddress::LocalHost && i.toIPv4Address()) {
            ipAddress = i.toString();
            break;
        }
    }
    // If we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->stato->setText(tr("Il server si trova qui\n———————————————\nindirizzo IP: %1\nporta: %2\n———————————————\n"
                            "Ora puoi connettere i client.")
                                 .arg(ipAddress).arg(tcpServer->serverPort()));
}

Server::~Server() {
    delete ui;
}

void Server::checkUser() {

    in.setDevice(active_socket);
    in.setVersion(QDataStream::Qt_4_0);

    in.startTransaction();
    QString credentials;
    QString reply;
    in >> credentials;

    if (!in.commitTransaction())
        return;


    //qui bisognerebbe controllare le credenziali, per verificare che funzioni lo scambio mi limito a rimandargliele indietro

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!active_socket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        out << QString(credentials);
        if (!active_socket->write(block)) {
            QMessageBox::information(this, tr("Server"), tr("Could not send message"));
        }
        active_socket->flush();
        connect(active_socket, &QAbstractSocket::disconnected,
                active_socket, &QObject::deleteLater);

    }
}

void Server::getConnectedSocket(){

    active_socket=tcpServer->nextPendingConnection();
    connect(active_socket, &QIODevice::readyRead, this, &Server::checkUser);
}