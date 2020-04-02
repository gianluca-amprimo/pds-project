#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <iostream>
#include <sstream>

#include "server.h"
#include "ui_server.h"
#include "db_operations.cpp"

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

        printConsole("Ripristino la sessione di rete precedente ...");
        networkSession->open();
    } else {
        sessionOpened();
    }
    // connect(tcpServer, &QTcpServer::newConnection, this, CALLBACK FOR NEW CONNECTION);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::getConnectedSocket);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sessionOpened() {
    int port = 4848;

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
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        printConsole("Impossibile avviare il server - " + tcpServer->errorString().toStdString(), true);
        close();
        return;
    }
    QString ipAddress;
#ifndef LOCALHOST
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const auto & i : ipAddressesList) {
        if (i != QHostAddress::LocalHost && i.toIPv4Address()) {
            ipAddress = i.toString();
            break;
        }
    }*/

    // If we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
#else
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
#endif
    printConsole("Il server è in funzione e si trova qui:  <u>" + ipAddress.toStdString() + ":" + QString::number(tcpServer->serverPort()).toStdString() + "</u>");
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
    printConsole("[" + active_socket->peerAddress().toString().toStdString() + ":" + QString::number(active_socket->peerPort()).toStdString() + "] " + credentials.toStdString());

    // TODO: qui bisognerebbe controllare le credenziali,
    //  per verificare che funzioni lo scambio mi limito a rimandargliele indietro
    std::string usr_pass = credentials.toStdString(); // converto QString in stringa standard

    // divide the string username_password in two separate string
    std::istringstream iss(usr_pass);
    std::string username, password;
    std::getline(iss, username, '_');
    iss >> password;
    // std::cout << username << std::endl << password << std::endl;

    // check the credentials
    QString loginResult;
    if(::checkUser(username, password))
        loginResult = "Success";
    else
        loginResult = "Login Failed";

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            printConsole("Socket TCP non valida", true);
            return;
        }
        if (!active_socket->isOpen()) {
            printConsole("Socket TCP non aperta", true);
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        out << QString(loginResult);
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
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


void Server::printConsole(std::string &&msg, bool err) {
    // Get the current time
    std::time_t t = std::time(nullptr);
    std::string timeStr;
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%T", std::localtime(&t));

    // Print information on the graphical console
    if(err)
        this->ui->console->insertHtml(QString::fromStdString(
                "<p style=\"color:red;\"><b>" + std::string(mbstr) + "</b> " + msg + "<br></p>"
                ));
    else
        this->ui->console->insertHtml(QString::fromStdString(
                "<p><b>" + std::string(mbstr) + "</b> " + msg + "<br></p>"
                ));
}
