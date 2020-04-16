#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <random>
#include <iostream>
#include <sstream>

#include "User.h"
#include "server.h"
#include "ui_server.h"
#include "db_operations.h"

static QString picturePath("../Pictures/");
std::array<std::string, 10> colors{"white", "red", "green", "blue",
                                   "cyan", "magenta", "yellow", "gray"};

/*
 * Constructor of the server
 */
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
    printConsole("Il server è in funzione e si trova qui:  <u>" + ipAddress.toStdString() + ":" +
                 QString::number(tcpServer->serverPort()).toStdString() + "</u>");
}


Server::~Server() {
    delete ui;
}


/*
 * Function to process a user request
 */
void Server::processUserRequest() {
    QTcpSocket *active_socket = (QTcpSocket *) sender();
    in.setDevice(active_socket);
    in.setVersion(QDataStream::Qt_4_0);

    //read the Json message received from client, from header understand what to do
    in.startTransaction();
    //QString qmessage;
    QByteArray jSmessage;
    std::string header;
    QJsonObject jSobject;
    //QString reply;
    in >> jSmessage;
    QJsonParseError parseError;
    // we try to create a json document with the data we received
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jSmessage, &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        // if the data was indeed valid JSON
        if (jsonDoc.isObject()) {
            jSobject = jsonDoc.object();
            header = jSobject["header"].toString().toStdString();
        } else {
            //TODO: error with the json do something (implement function for generic error message to client)
        }

    } else {
        //TODO: error with the json do something (implement function for generic error message to client)
    }

    if (!in.commitTransaction())
        return;
    printConsole("[" + active_socket->peerAddress().toString().toStdString() + ":" +
                 QString::number(active_socket->peerPort()).toStdString() + "] " + header);

    bool opResult;
    if (header == "log")
        opResult = Server::checkUser(jSobject, active_socket);
    if (header == "reg")
        opResult = Server::registerUser(jSobject, active_socket);
    if (header == "canc")
        opResult = Server::cancelUser(jSobject, active_socket);
    if (header == "upd")
        opResult = Server::updateUser(jSobject, active_socket);
    qDebug() << opResult;

}


void Server::getConnectedSocket() {
    //accetta la connessione al socket e prendi il socket connesso
    auto active_socket = tcpServer->nextPendingConnection();
    printConsole("I am accepting a new connection from socket "+std::to_string(active_socket->socketDescriptor()));
    //inserisci il socket nella lista dei socket attivi
    //active_sockets.push_back(active_socket);
    connect(active_socket, &QIODevice::readyRead, this, &Server::processUserRequest);
    connect(active_socket, &QAbstractSocket::disconnected, this, &Server::handleDisconnect);


}


void Server::printConsole(std::string &&msg, bool err) {
    // Get the current time
    time_t t = time(nullptr);
    std::string timeStr;
    char mbstr[100];
    strftime(mbstr, sizeof(mbstr), "%T", localtime(&t));

    // Print information on the graphical console
    if (err)
        this->ui->console->insertHtml(QString::fromStdString(
                "<p style=\"color:red;\"><b>" + std::string(mbstr) + "</b> " + msg + "<br></p>"
        ));
    else
        this->ui->console->insertHtml(QString::fromStdString(
                "<p><b>" + std::string(mbstr) + "</b> " + msg + "<br></p>"
        ));
}


/*
 * Function to check the user credentials and if they are log the user
 */
bool Server::checkUser(QJsonObject &data, QTcpSocket *active_socket) {

    // divide the string username_password in two separate string
    std::string username = data["username"].toString().toStdString();
    std::string password = data["password"].toString().toStdString();
    printConsole("Checking " + username + " " + password);

    // check the credentials
    QString loginResult;
    int queryResult = checkCredentials(username, password);
    if (queryResult == 1) {
        loginResult = "ok";
        QString un = QString::fromStdString(username);
        User u(un);
        auto it = activeUsers.begin();
        bool found = false;
        while (it !=
               activeUsers.end()) { //a user could open again the client and log again so first check if it's already there
            User user = it->first;
            if (user == u) {
                found = true;
                activeUsers[user].push_back(active_socket);
                break;
            }
            it++;
        }
        if (found != true) { //inserisci l'utente nella lista di quelli attualmente connessi
            std::list<QTcpSocket *> temp;
            temp.push_back(active_socket);
            activeUsers[u] = temp;

            // assegna colore random allo user
            std::random_device rd;
            std::string userColor = colors[rd() % 7];
            std::pair<User, std::string> userColorPair = std::make_pair(u, userColor);
            userColorMap.insert(userColorPair);
        }

        std::string user_list;
        for (std::pair<User, std::list<QTcpSocket *>> element : activeUsers) {
            User u = element.first;     // Accessing KEY from element
            std::string socket_list;    // Accessing VALUE from element

            for (auto s: element.second) {
                socket_list += std::to_string(s->socketDescriptor()) + " ";
            }
            user_list += u.getUsername().toStdString() + ": " + socket_list + " ";
        }
        printConsole("List of active users and their socket: " + user_list);

    } else if (queryResult == 0)
        loginResult = "unreg";
    else
        loginResult = "fail";

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            printConsole("Socket TCP non valida", true);
            return false;
        }
        if (!active_socket->isOpen()) {
            printConsole("Socket TCP non aperta", true);
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        QJsonObject message;
        // message["header"] = "log";
        // message["body"] = loginResult;
        message = prepareJsonWithFileList("log", loginResult, username);
        printConsole("Sending back " + message["header"].toString().toStdString() + " " +
                     message["body"].toString().toStdString());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
        }
        active_socket->flush();
    }
    return true;
}


/*
 * Function to register a new user
 *
 * Returns
 *      true if the user was added correctly
 *      false if something went wrong
 */
bool Server::registerUser(QJsonObject &data, QTcpSocket *active_socket) {

    // divide the string username_password_name_surname in  separate string
    std::string username = data["username"].toString().toStdString();
    std::string password = data["password"].toString().toStdString();
    std::string name = data["name"].toString().toStdString();
    std::string surname = data["surname"].toString().toStdString();
    QPixmap propic = pixmapFrom(data["propic"]);

    // load data in the DB and create the associated user if insertion works
    QString registrationResult;
    int queryResult = addUser(username, password, name, surname);
    if (queryResult == 1) {
        registrationResult = "ok";
        QString un = QString::fromStdString(username);
        User u(un);

        // inserisci l'utente nella lista di quelli attualmente connessi
        std::list<QTcpSocket *> temp;
        temp.push_back(active_socket);
        activeUsers[u] = temp;

        // save the propic chosen by the user using its username as username.png
        QFile file(picturePath + QString::fromStdString(username) + ".png");
        file.open(QIODevice::WriteOnly);
        propic.save(&file, "png", 100);

        // assegna colore random allo user
        std::random_device rd;
        std::string userColor = colors[rd() % 7];
        std::pair<User, std::string> userColorPair = std::make_pair(u, userColor);
        userColorMap.insert(userColorPair);
    }
    if (queryResult == -1)
        registrationResult = "fail";
    if (queryResult == 0)
        registrationResult = "alreadyreg";

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            printConsole("Socket TCP non valida", true);
            return false;
        }
        if (!active_socket->isOpen()) {
            printConsole("Socket TCP non aperta", true);
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        QJsonObject message;
        // message["header"] = "reg";
        // message["body"] = registrationResult;
        message = prepareJsonWithFileList("reg", registrationResult, username);
        printConsole("Sending back " + message["header"].toString().toStdString() + " " +
                     message["body"].toString().toStdString());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
        }
        active_socket->flush();
    }
    return true;
}


/*
 * Function to delete an existing user
 *
 * Returns:
 *      true if the user was cancelled correctly
 *      false if something went wrong
 */
bool Server::cancelUser(QJsonObject &data, QTcpSocket *active_socket) {

    // divide the string username_password_name_surname in  separate string
    std::string username = data["username"].toString().toStdString();
    std::string password = data["password"].toString().toStdString();
    printConsole("Trying to delete " + username + " " + password);

    // call function to delete user in db
    QString cancResult;
    //TODO: controlla che lo user non sia cosi balengo da cercare di
    // cancellare il suo account mentre è connesso su un altro client aperto
    int queryResult = deleteUser(username, password);
    if (queryResult == 1) {
        cancResult = "ok";
        QFile file(picturePath + QString::fromStdString(username) + ".png");
        file.remove();
    }
    if (queryResult == -1)
        cancResult = "fail";
    if (queryResult == 0)
        cancResult = "notpres";

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            printConsole("Socket TCP non valida", true);
            return false;
        }
        if (!active_socket->isOpen()) {
            printConsole("Socket TCP non aperta", true);
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        QJsonObject message;
        message["header"] = "canc";
        message["body"] = cancResult;
        printConsole("Sending back " + message["header"].toString().toStdString() + " " +
                     message["body"].toString().toStdString());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
        }
        active_socket->flush();
    }
    return true;
}


/*
 * Function to prepare the json to send to the user
 */
QJsonObject Server::prepareJsonWithFileList(QString header, QString result, std::string username) {
    int ret = readFiles();
    // TODO: if is not possible to read files do something
    QJsonObject message;
    std::tuple<std::string, std::string> personalInfo = getPersonalInfo(username);
    if (std::get<0>(personalInfo) == "db_error") {
        // TODO: problem in reading the credential send an error message to client and exit from function
    }

    message["header"] = header;
    message["body"] = result;
    message["name"] = QString::fromStdString((std::get<0>(personalInfo)));
    message["surname"] = QString::fromStdString((std::get<1>(personalInfo)));

    std::string url(picturePath.toStdString() + username + ".png");
    QPixmap img(QString::fromStdString(url));
    message["propic"] = jsonValFromPixmap(img);

    QJsonArray fileList;
    auto it = file_list.begin();
    for (auto t: file_list) {
        fileList.push_back(QString::fromStdString(std::get<0>(t)));
    }
    message.insert("File list", fileList);
    return message;
}


/*
 * Function to disconnect the user
 */
void Server::handleDisconnect() {
    QTcpSocket *disconnected_socket = (QTcpSocket *) sender();
    std::string currently_active;
    std::string user_list;
    for (std::pair<User, std::list<QTcpSocket *>> element : activeUsers) {
        User u = element.first;     // Accessing KEY from element
        std::string socket_list;    // Accessing VALUE from element

        for (auto s: element.second) {
            socket_list += std::to_string(s->socketDescriptor()) + " ";
            if (s == disconnected_socket) {
                activeUsers[u].remove(s);
                if (activeUsers[u].empty()) {
                    // rimuovi user da mappa user colore
                    userColorMap.erase(u);
                    activeUsers.erase(u);
                }
            }
        }

        user_list += u.getUsername().toStdString() + ": " + socket_list + " ";
    }
    printConsole("List of active users and their socket: " + user_list);
    disconnected_socket->deleteLater();

    //TODO: rimuovere socket dalla lista dei socket attivi, rimuovere lo user dalla mappa degli user attivi
    // se è il suo unico socket aperto, eventualmente chiudere il file se lo user era l’unico utente online (e non l'avesse chiuso)
}


QJsonValue Server::jsonValFromPixmap(const QPixmap &p) {
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    return {QLatin1String(encoded)};
}


QPixmap Server::pixmapFrom(const QJsonValue &val) {
    auto const encoded = val.toString().toLatin1();
    QPixmap p;
    p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
    return p;
}


/*
 * Function to update the password of the user
 */
bool Server::updateUser(QJsonObject &data, QTcpSocket *active_socket) {
    // divide the string username_password_name_surname in  separate string
    std::string username = data["username"].toString().toStdString();
    std::string password = data["password"].toString().toStdString();
    std::string newpassword = data["newpassword"].toString().toStdString();
    QPixmap propic = pixmapFrom(data["propic"]);

    QString updateResult;
    if (newpassword != "") {
        // update the password
        int queryResult = changePassword(username, password, newpassword);
        if (queryResult == 1) {
            updateResult = "ok";

            // save the propic chosen by the user using its username as username.png
            QFile file(picturePath + QString::fromStdString(username) + ".png");
            file.open(QIODevice::WriteOnly);
            propic.save(&file, "png", 100);
        }
        if (queryResult == -1)
            updateResult = "fail";
        if (queryResult == 0)
            updateResult = "wrongpass";
    } else {
        int queryResult = checkCredentials(username, password);
        if (queryResult == 1) {
            updateResult = "ok";

            // save the propic chosen by the user using its username as username.png
            QFile file(picturePath + QString::fromStdString(username) + ".png");
            file.open(QIODevice::WriteOnly);
            propic.save(&file, "png", 100);
        }
        if (queryResult == -1)
            updateResult = "fail";
        if (queryResult == 0)
            updateResult = "wrongpass";
    }

    if (active_socket != nullptr) {
        if (!active_socket->isValid()) {
            printConsole("Socket TCP non valida", true);
            return false;
        }
        if (!active_socket->isOpen()) {
            printConsole("Socket TCP non aperta", true);
            return false;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        QJsonObject message;
        message = prepareJsonWithFileList("upd", updateResult, username);
        printConsole("Sending back " + message["header"].toString().toStdString() + " " +
                     message["body"].toString().toStdString());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
        }
        active_socket->flush();
    }
    return true;
}
