#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <random>
#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>

#include "server.h"
#include "ui_server.h"
#include "db_operations.h"

static QString picturePath("../Pictures/");

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
        QSettings settings(QSettings::UserScope, QLatin1String("PiDiEsse [server]"));
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

        printConsole("Recovering previous network configuration");
        networkSession->open();
    } else {
        sessionOpened();
    }

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

        QSettings settings(QSettings::UserScope, QLatin1String("PiDiEsse [server]"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        printConsole("Not able to start the server - " + tcpServer->errorString(), true);
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
    printConsole((QString &&) ("Server is working:  <u>" + ipAddress + ":" +
                               QString::number(tcpServer->serverPort()) + "</u>"));
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

    // read the JSON message received from client, from header understand what to do
    // QString qmessage;
    QByteArray jSmessage;
    QString header;
    QJsonObject jSobject;
    QJsonArray jSarray;
    //QString reply;
    while(!in.atEnd()){
        in.startTransaction();
        in >> jSmessage;
        QJsonParseError parseError;
        // we try to create a json document with the data we received
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jSmessage, &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            // if the data was indeed valid JSON
            if (jsonDoc.isObject()) {
                jSobject = jsonDoc.object();
                header = jSobject["header"].toString();
            } else if(jsonDoc.isArray()){
               jSarray = jsonDoc.array();
               header = jSarray[0].toObject()["header"].toString();
               qDebug() << "header is " << header;

            }else{
                QJsonObject message = prepareJsonReply("error", "error", " ");
                sendMessage(message, active_socket);
                return;
            }

        } else {
            QJsonObject message=prepareJsonReply("error", "error", " ");
            sendMessage(message, active_socket);
            return;
        }

        if (!in.commitTransaction())
            return;

        printConsole("[" + active_socket->peerAddress().toString() + ":" +
                     QString::number(active_socket->peerPort()) + "] " + header);

        bool opResult;
        if (header == "log")
            opResult = Server::checkUser(jSobject, active_socket);
        if (header == "reg")
            opResult = Server::registerUser(jSobject, active_socket);
        if (header == "canc")
            opResult = Server::cancelUser(jSobject, active_socket);
        if (header == "upd")
            opResult = Server::updateUser(jSobject, active_socket);
        if (header == "refr")
            opResult = Server::refreshFileList(jSobject, active_socket);
        if (header == "newfile")
            opResult = Server::createFile(jSobject, active_socket);
        if (header == "openfile")
            opResult = Server::openFile(jSobject, active_socket);
        if (header == "savefile")
            opResult = Server::saveFileReq(jSobject, active_socket);
        if (header == "add1Char")
            opResult = Server::receiveChar(jSobject, active_socket);
        if (header == "addBatchChar")
            opResult = Server::receiveBatchChar(jSarray, active_socket);
        if (header == "delete1Char")
            opResult = Server::deleteChar(jSobject, active_socket);
        if (header == "deleteBatchChar")
            opResult = Server::deleteBatchChar(jSobject, active_socket);
        if (header == "sessionlogout")
            opResult = Server::closeFileReq(jSobject, active_socket);
        if (header=="shareFile")
            opResult = Server::shareFileReq(jSobject, active_socket);
        qDebug() << opResult;
    }
}


void Server::getConnectedSocket() {
    // accetta la connessione al socket e prendi il socket connesso
    auto active_socket = tcpServer->nextPendingConnection();
    printConsole((QString &&) ("I am accepting a new connection from socket " +
                               QString::fromStdString(std::to_string(active_socket->socketDescriptor()))));
    // inserisci il socket nella lista dei socket attivi
    // active_sockets.push_back(active_socket);
    // active_sockets.push_back(active_socket);
    connect(active_socket, &QIODevice::readyRead, this, &Server::processUserRequest);
    connect(active_socket, &QAbstractSocket::disconnected, this, &Server::handleDisconnect);


}

void Server::printConsole(QString msg, bool err) {
    // Get the current time
    time_t t = time(nullptr);
    QString timeStr;
    char mbstr[100];
    strftime(mbstr, sizeof(mbstr), "%T", localtime(&t));

    // Print information on the graphical console
    if (err)
        this->ui->console->insertHtml(QString(
                "<p style=\"color:red;\"><b>" + QString(mbstr) + "</b> " + msg + "<br></p>"
        ));
    else
        this->ui->console->insertHtml(QString(
                "<p><b>" + QString(mbstr) + "</b> " + msg + "<br></p>"
        ));
}


/*
 * Function to check the user credentials and if they are log the user
 */
bool Server::checkUser(QJsonObject &data, QTcpSocket *active_socket) {
    // divide the string username_password in two separate string
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    printConsole((QString &&) ("Checking " + username + " " + password));

    // check user credentials
    QString loginResult;
//    if (checkPasswordFormat(password)){
        int queryResult = checkCredentials(username.toStdString(), password.toStdString());
        if (queryResult == 1) {
            loginResult = "ok";

            // user cannot log on more than one terminal
            for (QString us: idleConnectedUsers.keys()){
                if (us == username){
                    loginResult = "fail";
                    printConsole((QString &&) ("User is already logged on another terminal"));
                }
            }

            if(loginResult == "ok"){
               idleConnectedUsers.insert(username, active_socket);
            }
        } else if (queryResult == 0)
            loginResult = "unreg";
        else
            loginResult = "fail";
//    } else
//        loginResult = "wrongPasswordFormat";

    // send message to the client with the result of the operation
    QJsonObject message;
    if(loginResult=="unreg" || loginResult=="fail")
        message = prepareJsonReply("log", loginResult, username, false, false, false);
    else
        message = prepareJsonReply("log", loginResult, username, true, true, true);
    printConsole("Sending back " + message["header"].toString() + " " +
                 message["body"].toString());
    sendMessage(message, active_socket);
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
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString name = data["name"].toString();
    QString surname = data["surname"].toString();
    QPixmap propic = pixmapFrom(data["propic"]);

    // load data in the DB and create the associated user if insertion works
    QString registrationResult;
    //    if(checkPasswordFormat(password)){
        int queryResult = addUser(username.toStdString(), password.toStdString(), name.toStdString(), surname.toStdString());
        if (queryResult == 1) {
            registrationResult = "ok";

            idleConnectedUsers.insert(username, active_socket);     // inserisci l'utente nella lista di quelli attualmente connessi

            QFile file(picturePath + username + ".png");            // save the propic chosen by the user using its username as username.png
            file.open(QIODevice::WriteOnly);
            propic.save(&file, "png", 100);
        }
        if (queryResult == -1)
            registrationResult = "fail";
        if (queryResult == 0)
            registrationResult = "alreadyreg";
//    } else
//        registrationResult = "wrongPasswordFormat";

    // send message to the client with the result of the operation
    QJsonObject message;
    if(registrationResult=="alreadyreg" || registrationResult=="fail")
        message = prepareJsonReply("reg", registrationResult, username, false, false, false);
    else
        message = prepareJsonReply("reg", registrationResult, username, true, true, true);
    message = prepareJsonReply("reg", registrationResult, username, true, true, true);
    printConsole("Sending back " + message["header"].toString() + " " +
                 message["body"].toString());
    sendMessage(message, active_socket);

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
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    printConsole("Trying to delete " + username + " " + password);

    // call function to delete user in db
    QString cancResult;
    bool flag=false;

    // if the user si connected, he cannot be deleted
    if (idleConnectedUsers.contains(username))
        flag = true;

    printConsole("While you are trying to delete the following users are online...");
    for (QString user : idleConnectedUsers.keys())
        printConsole(user);

    if(flag){
        cancResult="fail";
    }
    else {
//    if(checkPasswordFormat(password)){
        int queryResult = deleteUser(username.toStdString(), password.toStdString());
        if (queryResult == 1) {
            cancResult = "ok";
            QFile file(picturePath + username + ".png");
            file.remove();
        }
        if (queryResult == -1)
            cancResult = "fail";
        if (queryResult == 0)
            cancResult = "notpres";
//    } else
//        cancResult = "wrongPasswordFormat";
    }

    // send message to the client with the result of the operation
    QJsonObject message;
    message = prepareJsonReply("canc", cancResult, username);
    printConsole("Sending back " + message["header"].toString() + " " +
                 message["body"].toString());
    sendMessage(message, active_socket);
    return true;
}

/*
 * Function to prepare the json to send to the user
 */
QJsonObject Server::prepareJsonReply(QString header, QString result, QString username, bool propic, bool filelist, bool personal_info) {

    QJsonObject message;
    if(header=="error"){
        message["header"]="error";
        return message;
    }

    message["header"] = header;
    message["body"] = result;

    if(filelist){
        int ret = readFiles(username.toStdString());
        if (ret==-1) {
            message["header"]="error";
            return message;
        }
        QJsonArray fileList;
        auto it = file_list.begin();
        for (auto t: file_list) {
            fileList.push_back(QString::fromStdString(std::get<0>(t)));
        }
        message.insert("File list", fileList);
    }

    if(personal_info){
        std::tuple<QString, QString> personalInfo = getPersonalInfo(username.toStdString());
        if (std::get<0>(personalInfo) == "db_error" ) {
            message["header"]="error";
            return message;
        }
        message["name"] = std::get<0>(personalInfo);
        message["surname"] = std::get<1>(personalInfo);
    }

    if (propic){
        QString url(picturePath + username + ".png");
        QPixmap img(url);
        message["propic"] = jsonValFromPixmap(img);
    }

    return message;
}

// the user is closing the socket and disconnecting from the server
void Server::handleDisconnect() {
    auto *disconnected_socket = (QTcpSocket *) sender();
    QString currently_active;
    QString user_list;

    // un utente può essere connesso solo con un editor, quindi non bisogna prendere
    // in considerazione il caso in cui ci siano più socket attivi
    // find the username of the user
    for (const QString& user : idleConnectedUsers.keys()) {
        if (idleConnectedUsers.value(user) == disconnected_socket) {
            qDebug() << "User " << user << " disconnected from the server";
            idleConnectedUsers.remove(user);
            for(const auto session : active_sessions) {
                auto it = session->userMap.find(user);
                if(it != session->userMap.end()) {
                    removeUserFromSession(session, user, std::optional<std::shared_ptr<QJsonObject>>());
                    break;
                }
            }
        }
    }
    // remove the user from the map idleConnectedUsers
    disconnected_socket->deleteLater();
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
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString newpassword = data["newpassword"].toString();
    QPixmap propic = pixmapFrom(data["propic"]);

    QString updateResult;
    if (!newpassword.isEmpty()) {
//        if(checkPasswordFormat(password) && checkPasswordFormat(newpassword)){
            // update the password
            int queryResult = changePassword(username.toStdString(), password.toStdString(), newpassword.toStdString());
            if (queryResult == 1) {
                updateResult = "ok";

                // save the propic chosen by the user using its username as username.png
                QFile file(picturePath + username + ".png");
                file.open(QIODevice::WriteOnly);
                propic.save(&file, "png", 100);
            }
            if (queryResult == -1)
                updateResult = "fail";
            if (queryResult == 0)
                updateResult = "wrongpass";
//        } else
//            updateResult = "wrongNewPasswordFormat";
    } else {
//        if(checkPasswordFormat(password)){
            int queryResult = checkCredentials(username.toStdString(), password.toStdString());
            if (queryResult == 1) {
                updateResult = "ok";

                // save the propic chosen by the user using its username as username.png
                QFile file(picturePath + username + ".png");
                file.open(QIODevice::WriteOnly);
                propic.save(&file, "png", 100);
            }
            if (queryResult == -1)
                updateResult = "fail";
            if (queryResult == 0)
                updateResult = "wrongpass";
//        } else
//            updateResult = "wrongPasswordFormat";
    }

    // send message to the client with the result of the operation
    QJsonObject message;
    message = prepareJsonReply("upd", updateResult, username, true, true, true);
    printConsole("Sending back " + message["header"].toString() + " " +
                 message["body"].toString());
    sendMessage(message, active_socket);

    return true;
}

/*
 * Function to check if the password is in the correct format
 *
 * Returns:
 *      true if the password is between 8 and 16 characters and
 *      if it contains three between number, special characters,
 *      uppercase letter and lowercase letter
 *
 *      false otherwise
 */
bool Server::checkPasswordFormat(QString password){
	bool uppercase;
	bool lowercase;
	bool special;
	bool number;
	bool space;
	
	int correct = 0;
	
	QRegularExpression uppercase_regex {"[A-Z]+"};
	QRegularExpression lowercase_regex {"[a-z]+"};
	QRegularExpression special_regex {"[._!?@]+"};
	QRegularExpression number_regex {"[0-9]+"};
	QRegularExpression space_regex {"[ ]+"};
	
	if(password.length() < 8 || password.length() > 16) {
		return false;
	} else {
		uppercase = uppercase_regex.match(password).hasMatch();
		lowercase = lowercase_regex.match(password).hasMatch();
		special = special_regex.match(password).hasMatch();
		number = number_regex.match(password).hasMatch();
		space = space_regex.match(password).hasMatch();
		
		if (space) return false;
		if(uppercase) correct++;
		if(lowercase) correct++;
		if(special) correct++;
		if(number) correct++;
		
		if(correct >= 3)
			return true;
		else
			return false;
	}
}

template<typename QJson>
void Server::sendMessage(QJson message, QTcpSocket *active_socket){
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

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        if (!active_socket->write(block)) {
            printConsole("Impossibile rispondere al client", true);
        }
        active_socket->flush();
    }
    else{
        printConsole("Wrong! You are trying to write to a non existing socket!", true);
    }
}

bool Server::refreshFileList(QJsonObject &data, QTcpSocket *active_socket) {
	QString username = data["username"].toString();
	QString refreshResult("ok");
	QJsonObject message;
	message = prepareJsonReply("refr", refreshResult, username, false, true, false);
	printConsole("Sending back " + message["header"].toString() + " " +
	             message["body"].toString());
	sendMessage(message, active_socket);
	
	return true;
}

bool Server::createFile(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString();
    QString username = data["username"].toString();
    QJsonObject message;

    int result = addFile(filename.toStdString(), username.toStdString());
    if (result == -1) {
        message["header"] = "newfile";
        message["body"] = "internal_error";
        sendMessage(message, active_socket);
        printConsole("Internal server error while <i>" + username + "</i> was creating file <i>" + filename + "</i>", true);
        return false;
    }
    if (result == 0) {
        message["header"] = "newfile";
        message["body"] = "existing_file";
        sendMessage(message, active_socket);
        printConsole("<i>" + username + "</i> has tried to create file <i>" + filename + "</i>, but it already exists", true);
        return false;
    }
    message["header"] = "newfile";
    message["body"] = "ok";
    openFile(data, active_socket);
    return true;
}

bool Server::openFile(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString();
    QString username = data["username"].toString();
    QString editorId;

    QJsonObject message;
    message["header"] = "openfile";

    if(idleConnectedUsers.contains(username)){
        qDebug() << username << " is opening file : " << filename;
        int result = checkIfFileExists(filename.toStdString());
        if (result == -1) {
            message["body"] = "internal_error";
            sendMessage(message, active_socket);
            printConsole("Internal server error while <i>" + username + "</i> was reading file <i>" + filename + "</i>", true);
            return false;
        }
        if (result == 0) {
            message["body"] = "not_existing_file";
            sendMessage(message, active_socket);
            printConsole("<i>" + username + "</i> has tried to open file <i>" + filename + "</i>, but it doesn't exist", true);
            return false;
        }

        QString listUsers("List of users currently connected to the server: ");
        for(QString user: idleConnectedUsers.keys()){
            listUsers.append(user + " ");
        }
        qDebug() << listUsers;

        auto session = active_sessions.find(filename);
        // session already exists, simply add the user to it
        if(session != active_sessions.end()){
            qDebug() << "User " << username << " joining existing session for file " << filename;
            editorId = (*session)->getEditorPrefix() + std::to_string((*session)->getEditorCounter()).c_str();
            // add user to session
            (*session)->addUserToSession(username, editorId);
            qDebug() << "Session has " << (*session)->getEditorCounter() << " users connected";

            // choose a color for the user
            QColor color = generateColor();
            qDebug() << "Color associated to user " << username << " is: " << color.name();
            (*session)->userMap.insert(username, color.name());

            printConsole("Adding user <i>" + username +
                                "</i> with editorId = <i>" + editorId +
                                "</i> to session for file <i>" + filename + "</i>");

            // send message to the client with the result of the operation
            QByteArray byteArrayBuffer;
            QDataStream outStream(&byteArrayBuffer, QIODevice::WriteOnly);

            outStream << (*session)->getSymbolsByPosition().values();
            message["editorId"] = editorId;
            message["body"] = "existing_session";
            message["filename"] = data["filename"];
            message["content"] = QLatin1String(byteArrayBuffer.toBase64());
            sendMessage(message, active_socket);
            printConsole("Sending file <i>" + filename + "</i> for user <i>" + username + "</i>");

            // send user color to other users
            sendColors(filename, username);
            return true;
        } else {
            // session doesn't exist, create it and put first user
            qDebug() << "User " << username << " is asking to create a new sessions for file " << filename;
            auto newSession = std::make_shared<Session>(filename);
            active_sessions.insert(filename, newSession);
            editorId = newSession->getEditorPrefix() + std::to_string(newSession->getEditorCounter()).c_str();

            // add user to session
            newSession->addUserToSession(username, editorId);
            qDebug() << "Now there are " << newSession->getEditorCounter() << " users connected to the session for file " << filename;

            // choose a color for the user
            QColor color = generateColor();
            qDebug() << "Color associated to user " << username << " is: " << color.name();
            newSession->userMap.insert(username, color.name());
            // here non need to send colors, there is only one user connected

            printConsole("Creating new session for file <i>" + filename +
                                "</i> by user <i>" + username +
                                "</i> with editorId = <i>" + editorId + "</i>");


            QFile fi((QString::fromStdString(fs_root) + filename));
            fi.open(QIODevice::ReadOnly);
            QByteArray byteArrayBuffer;
            QDataStream stream(&byteArrayBuffer, QIODevice::ReadOnly);
            if(fi.isOpen()){
                byteArrayBuffer = fi.readAll();
                fi.close();
            } else {
                message["body"] = "internal_error";
                sendMessage(message, active_socket);
                printConsole("Internal server error while <i>" + username + "</i> was reading file <i>" + filename + "</i>", true);
                return false;
            }

            QList<Symbol> listOfSymbols;
            stream >> listOfSymbols;
            for(auto& sym: listOfSymbols) {
                newSession->addSymbol(sym);
                qDebug() << "Inserting symbol with " << sym.getCharacter() << " and position " << sym.getPosition().getStringPosition();
            }

            message["editorId"] = editorId;
            message["body"] = "new_session";
            message["filename"] = data["filename"];
            message["content"] = QLatin1String(byteArrayBuffer.toBase64());
            sendMessage(message, active_socket);
            printConsole("Sending file <i>" + filename + "</i> for user <i>" + username + "</i>");
            return true;
        }
    }
    message["body"] = "bad_request";
    sendMessage(message, active_socket);
    printConsole("User <i>" + username + "</i> is trying to open file <i>" + filename + "</i> but it's not connected to the server");
    return false;
}

// removing a user from a session
// and eventually destroy the session
// if there's no connected user anymore
bool Server::removeUserFromSession(std::shared_ptr<Session> session, const QString& username, std::optional<std::shared_ptr<QJsonObject>> returnMessage) {
    // qDebug() << "Session for file " << filename << " has " << session->getEditorCounter() << " users connected";
    // remove user from the session
    qDebug() << "Removing user " << username << " from session for file " << session->getFilename();
    QString editorId = session->removeUserFromSession(username);
    printConsole("Removing from session for file <i>" + session->getFilename() + "</i> user <i>" + username + "</i> with editorId = <i>" + editorId + "</i>");
    // qDebug() << "Session for file " << filename << " now has " << session->getEditorCounter() << " users connected";

    // if editorCounter == 0 destroy the session
    // remove session from map active_sessions
    if(session->getEditorCounter() == 0) {
        qDebug() << "Destroying session for file " << session->getFilename() << " from the list of active ones";
        // destroy the session
        active_sessions.remove(session->getFilename());

        QString listOfSessions;
        for(const QString& file: active_sessions.keys())
            listOfSessions.append(file + " ");
        qDebug() << "Checking the list of currently active sessions : " << listOfSessions;

        // saving the file
        QByteArray byteArrayBuffer;
        QDataStream outStream(&byteArrayBuffer, QIODevice::WriteOnly);
        outStream << session->getSymbolsById().values();
        if(!saveFile(session->getFilename(), byteArrayBuffer, returnMessage))
            return false;
    } else
        sendColors(session->getFilename(), username);

    if(returnMessage.has_value())
        (*returnMessage.value())["body"] = "ok";
    return true;
}

// function to remove a user from the session
// close the session in case it is the last user
bool Server::closeFileReq(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString();
    QString username = data["username"].toString();
    auto message = std::make_shared<QJsonObject>();
    (*message)["header"] = "closefile";

    // extract user from map user tcp connections
    qDebug() << username << " is closing file " << filename;

    // check in case someone is sending a bad request
    // session doesn't exist, simply do nothing
    if(idleConnectedUsers.contains(username)){
        auto it = active_sessions.find(filename);
        if(it != active_sessions.end()){
            // the session exists
            if(!removeUserFromSession(*it, username, message)) {
                sendMessage(*message, active_socket);
                return false;
            }
        } else {
            // the session doesn't exist: display error message and nothing else
            qDebug() << "The session doesn't exist, something went wrong";
            printConsole("User <i>" + username + "</i> is trying to close the session for file <i>" + filename + "</i> but it doesn't exist");
            // send error message to client
            (*message)["body"] = "error_file";
            sendMessage(*message, active_socket);
            return false;
        }
        // send message to client
        sendMessage(*message, active_socket);
        return true;
    }
    // bad request, user not connected
    qDebug() << "User " << username << " is trying to close file " << filename << " but it's not connected to the server";
    printConsole("User <i>" + username + "</i> is trying to close file <i>" + filename + "</i> but it's not connected to the server");
    // send error message to client
    (*message)["body"] = "bad_request";
    sendMessage(*message, active_socket);
    return false;
}

bool Server::saveFile(const QString& filename, const QByteArray& content, std::optional<std::shared_ptr<QJsonObject>> returnMessage) {
    int result = checkIfFileExists(filename.toStdString());
    if (result == -1) {
        if(returnMessage.has_value())
            (*returnMessage.value())["body"] = "internal_error";
        printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
        return false;
    }
    if (result == 0) {
        if(returnMessage.has_value())
            (*returnMessage.value())["body"] = "not_existing_file";
        printConsole("Trying to save file <i>" + filename + "</i>, but it doesn't exist anymore", true);
        return false;
    }

    QFile fo((QString::fromStdString(fs_root) + filename));
    fo.open(QIODevice::WriteOnly);
    if(fo.isOpen()){
        fo.write(content);
        fo.close();
    } else {
        if(returnMessage.has_value())
            (*returnMessage.value())["body"] = "internal_error";
        printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
        return false;
    }
    if(returnMessage.has_value()){
        (*returnMessage.value())["body"] = "ok";
        (*returnMessage.value())["filename"] = filename;
    }
    printConsole("<i>" + filename + "</i> correctly saved");
    return true;
}

bool Server::saveFileReq(QJsonObject &data, QTcpSocket *active_socket) {
    auto filename = data["filename"].toString();
    auto content = QByteArray::fromBase64(data["content"].toString().toLatin1());
    auto message = std::make_shared<QJsonObject>();
    (*message)["header"] = "savefile";

    qDebug() << "Saving file " << filename;
    bool res = saveFile(filename, content, message);
    sendMessage(*message, active_socket);
    if(!res)
        return false;
    return true;
}

bool Server::receiveChar(QJsonObject &data, QTcpSocket *active_socket) {
    /* message structure:
     * - header
     * - filename
     * - charId
     * - unicode
     * - position
     * - format
     */

    auto format = QByteArray::fromBase64(data["format"].toString().toLatin1());
    QDataStream inFormatStream(&format, QIODevice::ReadOnly);

    QString filename = data["filename"].toString().toLatin1();

    QByteArray symbolInBytes;
    QDataStream outSymbol(&symbolInBytes, QIODevice::WriteOnly);

    QJsonObject message;
    QTextCharFormat charFormat;

    // retrieve session from filename
    auto session = this->active_sessions.value(filename);

    // retrieve data for symbol from message

    // compute charId
    QString editorId = data["editorId"].toString();
    int counter = session->getSymbolsById().size();
    int localCharIdLen = std::to_wstring(counter).length();
    QString localCharId = QString(6 - localCharIdLen, '0') + QString::number(counter);

    QString charId = editorId + "_" + localCharId;

    FracPosition fp1, fp2, symbolFp;
    int position = data["position"].toInt();
    QString unicode = data["unicode"].toString();

    // deserialize format
    inFormatStream >> charFormat;
    qDebug() << "Received char " << unicode << " at position " << position;

    // compute Frac position
    if(position == 0){
        // insert at the head
        if(session->getSymbolsByPosition().empty()){
            // no symbols inserted yet
            symbolFp = "1";
        }else{
            // inserting before the first symbol
            symbolFp = session->getSymbolsByPosition().keys().at(0);
            symbolFp = symbolFp.divideByTwo();
        }
    }else if(position >= session->getSymbolsByPosition().size()){
        // insert at the back
        symbolFp = session->getSymbolsByPosition().keys().last() + one;
    }else{
        // insert in the middle
        fp1 = session->getSymbolsByPosition().keys().at(position-1);
        fp2 = session->getSymbolsByPosition().keys().at(position);
        symbolFp = fp1+fp2;
        symbolFp = symbolFp.divideByTwo();

    }

    // create symbol from message

    Symbol symbol(unicode[0], charId, symbolFp, charFormat);
    session->addSymbol(symbol);
    outSymbol << symbol;
    message["header"] = "add1Symbol";
    message["symbol"] = QLatin1String(symbolInBytes.toBase64());

    // now send symbol to all the other editors
    for(QString user : session->userEditorId.keys()){
        qDebug() << "Sending symbol" << symbol.getCharacter() << "to" << session->userEditorId.value(user);
        sendMessage(message, this->idleConnectedUsers.value(user));
    }

    return true;
}

bool Server::deleteChar(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString().toLatin1();
    qDebug() << "filename is: " << filename;

    QString symId = data["charId"].toString();
    QString symPos;

    auto session = this->active_sessions.value(filename);

    // acquire symbol
    QJsonObject message;
    message["header"] = "delete1Symbol";
    message["id"] = symId;
    QString stringPosition = session->getSymbolsById().value(symId).getPosition().getStringPosition();
    message["position"] = stringPosition;

    // now send deletion to all the other editors
    try {
        session->removeSymbol(symId);
    }catch (NonExistingSymbol &e){
        return false;
    }
    for(QString user : session->userEditorId.keys()){
        sendMessage(message, this->idleConnectedUsers.value(user));
    }

    return true;
}

QColor Server::generateColor(){
    std::random_device rd;

    int r = rd() % 256; // generate a random number between 0 and 255 for red
    int g = rd() % 256; // generate a random number between 0 and 255 for green
    int b = rd() % 256; // generate a random number between 0 and 255 for blue

    QColor color = QColor(r, g, b);
    return color;
}

bool Server::deleteBatchChar(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString().toLatin1();
    qDebug() << "filename is: " << filename;
    QVector<QString> symbolsInRange;
    QHash<QString, FracPosition> symbolsPosition;

    auto inSymbolsBytes = QByteArray::fromBase64(data["idsToDelete"].toString().toLatin1());
    QByteArray symbolPositionBytes;
    QDataStream outSymbolPositionStream(&symbolPositionBytes, QIODevice::WriteOnly);
    QDataStream inSymbolBytesStream(&inSymbolsBytes, QIODevice::ReadOnly);
    inSymbolBytesStream >> symbolsInRange;

    auto session = this->active_sessions.value(filename);
    // we have to retrieve all the symbol ids
    for(QString id: symbolsInRange){
        FracPosition fp = session->getSymbolsById().value(id).getPosition();
        symbolsPosition.insert(id, fp);
    }

    outSymbolPositionStream << symbolsPosition;

    QJsonObject message;
    message["header"] = "deleteBatchSymbol";
    message["idsAndPositions"] = QLatin1String(symbolPositionBytes.toBase64());

    // now send deletion to all other editors
    try {
        session->removeBatchSymbol(symbolsPosition);
    }catch (NonExistingSymbol &e){
        return false;
    }
    for(QString user : session->userEditorId.keys()){
        sendMessage(message, this->idleConnectedUsers[user]);
    }

    return true;
}

bool Server::receiveBatchChar(QJsonArray &data, QTcpSocket *active_socket) {
    /* message structure:
     * - metadata
     *  - header
     *  - filename
     *  - editorId
     *  - formatLength
     *  - length
     *  - formats
     *  symbols here:
     * - charId
     * - unicode
     * - position
     */

    // extrapolate metadata from the array
    QJsonObject metadata = data[0].toObject();
    QJsonObject metadataOut;
    QByteArray symbolInBytes;
    QJsonArray message;

    qDebug() << "Seems like the editor wants to paste" << metadata["length"] << "characters";
    auto formats = QByteArray::fromBase64(metadata["formats"].toString().toLatin1());
    QDataStream inFormatStream(&formats, QIODevice::ReadOnly);
    QString filename = metadata["filename"].toString().toLatin1();
    int arrayLenght = metadata["length"].toInt();
    QVector<QTextCharFormat> charFormats;

    metadataOut["header"] = "addBatchSymbol";
    metadataOut["length"] = arrayLenght;
    metadataOut["formatLength"] = metadata["formatLength"];
    metadataOut["formats"] = metadata["formats"];

    message.push_front(metadataOut);

    inFormatStream >> charFormats;
    // prepare session
    auto session = this->active_sessions.value(filename);


    // cycle through the array

    for(int i = 0; i < arrayLenght; i++){
        // take the object from the array
        QJsonObject singleChar = data[i+1].toObject();
        QJsonObject singleCharOut;
        FracPosition fp1, fp2, symbolFp;
        int position = singleChar["position"].toInt();
        QString unicode = singleChar["unicode"].toString();

        QString editorId = metadata["editorId"].toString();
        int counter = session->getSymbolsById().size();
        int localCharIdLen = std::to_wstring(counter).length();
        QString localCharId = QString(6 - localCharIdLen, '0') + QString::number(counter);

        QString charId = editorId + "_" + localCharId;

        // assign frac position
        if(position == 0){
            // insert at the head
            if(session->getSymbolsByPosition().empty()){
                // no symbols inserted yet
                symbolFp = "1";
            }else{
                // inserting before the first symbol
                symbolFp = session->getSymbolsByPosition().keys().at(0);
                symbolFp = symbolFp.divideByTwo();
            }
        }else if(position >= session->getSymbolsByPosition().size()){
            // insert at the back
            symbolFp = session->getSymbolsByPosition().keys().last() + one;
        }else{
            // insert in the middle
            fp1 = session->getSymbolsByPosition().keys().at(position-1);
            fp2 = session->getSymbolsByPosition().keys().at(position);
            symbolFp = fp1+fp2;
            symbolFp = symbolFp.divideByTwo();

        }
        qDebug() << "Received char " << unicode << "at position" << position;

        // build symbol from received data
        Symbol symbol(unicode[0], charId, symbolFp, charFormats[0]);
        session->addSymbol(symbol);

        // rebuild another JSON object with frac position to send
        singleCharOut["fracPosition"] = symbolFp.getStringPosition();
        singleCharOut["unicode"] = unicode;
        singleCharOut["charId"] = charId;

        message.push_back(singleCharOut);
    }

    // now send symbol to all other editors
    for(QString user : session->userEditorId.keys()){
        qDebug() << "Sending symbols" <<  "to" << user;
        sendMessage(message, this->idleConnectedUsers[user]);
    }

    return true;
}

void Server::sendColors(QString filename, QString username){
    auto session = this->active_sessions.value(filename);

    QJsonObject message;
    message["header"] = "colors";
    if (session->userMap.contains(username)){
        // the user opened the file
        // 1 send the other user his name and his color
        // 2 send him all the other users username and color

        // 1
        for (QString user : session->userMap.keys()){
            if (user != username){
                QTcpSocket *socket = this->idleConnectedUsers.value(user);
                QString color = session->userMap.value(username);

                // send user and color_position
                message["username"] = username;
                message["color"] = color;
                if (socket != nullptr)
                    sendMessage(message, socket);
            }
        }

        // 2
        QTcpSocket *socket = this->idleConnectedUsers.value(username);
        for (QString user : session->userMap.keys()){
            if (user != username){
                QString color = session->userMap.value(user);

                // send user and color_position
                message["username"] = user;
                message["color"] = color;
                if (socket != nullptr)
                    sendMessage(message, socket);
            }
        }
    } else {
        // user has just been disconnected
        // send the update to all the other connected users
        for (QString user : session->userMap.keys()){
            QTcpSocket *socket = this->idleConnectedUsers.value(user);
            QString color = "-1";       // color == -1 signal the log out of the user

            // send user and color_position
            message["username"] = username;
            message["color"] = color;
            if (socket != nullptr)
                sendMessage(message, socket);
        }
    }

    qDebug() << "Sending userMap for file: " << filename;
}

bool Server::shareFileReq(QJsonObject data, QTcpSocket *active_socket) {
    QString link = data["link"].toString();
    QString filename=link.split("/").at(1); //take only filename
    QString username = data["username"].toString();
    QJsonObject message;

    qDebug()<<"trying to get access to file "<<filename;
    int result = shareFiles(username.toStdString(), filename.toStdString());
    if (result == -1) {
        message["header"] = "shareFile";
        message["body"] = "internal_error";
        sendMessage(message, active_socket);
        printConsole("Internal server error while <i>" + username + "</i> was trying to get access to file <i>" + filename + "</i>", true);
        return false;
    }
    if (result == 0) {
        message["header"] = "shareFile";
        message["body"] = "non_existing_file_or_user";
        sendMessage(message, active_socket);
        printConsole("<i>" + username + "</i> has tried to have access to file <i>" + filename + "</i>, but user or file does not exists", true);
        return false;
    }
    if (result==2){
        //user access already granted
        message["header"] = "shareFile";
        message["body"] = "already_granted";
        sendMessage(message, active_socket);
        printConsole("<i>" + username + "</i> has tried to get access to file <i>" + filename + "</i>, but it already has", true);
        return false;
    }

    //everything is ok, access is granted so send the file  to the user
    message["username"]=username;
    message["filename"]=filename;
    openFile(message, active_socket);
    return true;
}

