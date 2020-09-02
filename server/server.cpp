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
    in.startTransaction();
    // QString qmessage;
    QByteArray jSmessage;
    QString header;
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
            header = jSobject["header"].toString();
        } else {
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
        opResult = Server::saveFile(jSobject, active_socket);
    if (header == "symbol")
        opResult = Server::receiveSymbol(jSobject, active_socket);
    if (header == "delSymbol")
        opResult = Server::deleteSymbol(jSobject, active_socket);
    if (header == "sessionlogout")
        opResult = Server::closeFile(jSobject, active_socket);
    if (header == "position")
        opResult = Server::updatePosition(jSobject, active_socket);

    qDebug() << opResult;
}


void Server::getConnectedSocket() {
    // accetta la connessione al socket e prendi il socket connesso
    auto active_socket = tcpServer->nextPendingConnection();
    printConsole((QString &&) ("I am accepting a new connection from socket " +
                               QString::fromStdString(std::to_string(active_socket->socketDescriptor()))));
    // inserisci il socket nella lista dei socket attivi
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
        int ret = readFiles();
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

/*
 * Function to disconnect the user
 */
void Server::handleDisconnect() {
    QTcpSocket *disconnected_socket = (QTcpSocket *) sender();
    QString currently_active;
    QString user_list;
    // un utente può essere connesso solo con un editor, quindi non bisogna prendere
    // in considerazione il caso in cui ci siano più socket attivi

    printConsole("List of active users and their socket: ");
    // find the username of the user
    QString username;
    for (QString user : idleConnectedUsers.keys()) {
        printConsole(user);
        if (idleConnectedUsers.value(user) == disconnected_socket)
            username = user;
    }
    // remove the user from the map idleConnectedUsers
    idleConnectedUsers.remove(username);

    disconnected_socket->deleteLater();

    // TODO: controllare se l'utente era connesso ad una sessione, quindi disconnetterlo dalla sessione
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

void Server::sendMessage(QJsonObject message, QTcpSocket *active_socket){
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
        printConsole("Sending back " + message["header"].toString() + " " +
                     message["body"].toString());

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
    Session session(filename);

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
    message["body"] = "file_created";
    openFile(data, active_socket);
    return true;
}

bool Server::openFile(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString();
    QString username = data["username"].toString();
    QString editorId;

    QJsonObject message;
    message["header"] = "openfile";

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

    qDebug() << "Opening file with" << username;
    for(QString user: idleConnectedUsers.keys()){
        qDebug() << user << "is connected to the server";
    }

    // check in case someone is sending a bad request
    // session already exists, simply add the user to it
    if(active_sessions.contains(filename)){
        Session *session = active_sessions.value(filename);
        if(idleConnectedUsers.contains(username)){
            qDebug() << "User" << username << "is connected, can proceed";
            editorId = session->getEditorPrefix() + QString(session->getEditorCounter());

            // add user to session
            session->addUserToSession(username, editorId);
            qDebug() << "Session has " << session->getEditorCounter() << " users connected";
            if (session->getEditorCounter() == 2){
                // create a timer associated to the session
                QTimer *timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, [=](){sendColors(filename);});
                timer->start(2000);
                this->timers.insert(filename, timer);
            }

            // choose a color for the user
            QColor color = generateColor();
            // TODO: for now initial position will be end of the file
            //  check if correct, later will be updated
            //  if the user moves the cursor
            int position = 0;
            QString color_pos = color.name() + "_" + position;
            qDebug() << "Color associated to user " << username << " is: " << color.name();
            session->userMap.insert(username, color_pos);
        }

        printConsole("Adding to session new user: " + username + " \nand editorId = " + editorId);

        // send message to the client with the result of the operation
        QByteArray byteArrayBuffer;
        QDataStream outStream(&byteArrayBuffer, QIODevice::ReadWrite);
        outStream << session->getSymbols().values();

        message["editorId"] = editorId;
        message["body"] = "existing_session";
        message["filename"] = data["filename"];
        message["content"] = QLatin1String(byteArrayBuffer.toBase64());
        sendMessage(message, active_socket);
        printConsole("Sending file <i>" + filename + "</i> for user <i>" + username + "</i>");
        return true;
    } else {
        // session doesn't exist, create it and put first user
        auto *fileSession = new Session(filename);
        if(idleConnectedUsers.contains(username)){
            qDebug() << "User is connected, can proceed";
            editorId = fileSession->getEditorPrefix() + QString(fileSession->getEditorCounter());

            // add user to session
            fileSession->addUserToSession(username, editorId);
            qDebug() << "Now there are " << fileSession->getEditorCounter() << " users connected";

            // choose a color for the user
            QColor color = generateColor();
            // TODO: for now initial position will be 0
            //  check if correct, later will be updated
            //  if the user moves the cursor
            int position = 0;
            QString color_pos = color.name() + "_" + position;
            qDebug() << "Color associated to user " << username << " is: " << color.name();
            fileSession->userMap.insert(username, color_pos);
        }

        printConsole("Creating new session with new user: " + username + " \nand editorId = " + editorId);
        // register new session in the server
        active_sessions.insert(filename, fileSession);

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
        for(auto& sym: listOfSymbols)
            fileSession->addSymbol(sym);

        message["editorId"] = editorId;
        message["body"] = "new_session";
        message["filename"] = data["filename"];
        message["content"] = QLatin1String(byteArrayBuffer.toBase64());
        sendMessage(message, active_socket);
        printConsole("Sending file <i>" + filename + "</i> for user <i>" + username + "</i>");
        return true;
    }
}

// function to remove a user from the session
// close the session in case is the last user
bool Server::closeFile(QJsonObject &data, QTcpSocket *active_socket) {
    QString filename = data["filename"].toString();
    QString username = data["username"].toString();
    QJsonObject message;
    message["header"] = "closefile";

    // extract user from map user tcp connections
    qDebug() << "Closing file with " << username;

    // check in case someone is sending a bad request
    // session doesn't exist, simply do nothing
    if(active_sessions.contains(filename)){ // the session exists
        Session *session = active_sessions.value(filename);
        if(idleConnectedUsers.contains(username)){
            qDebug() << "User " << username << "is connected, can proceed";
            qDebug() << "session has " << session->getEditorCounter() << " users connected";

            // remove user from the session
            session->removeUserFromSession(username);
            qDebug() << "session now has " << session->getEditorCounter() << " users connected";

            // remove user from userMap
            session->userMap.remove(username);
        }
        printConsole("Removing from session user: " + username + " \nand editorId = " + session->userEditorId.value(username));

        // if editorCounter == 1 il timer per mandare i colori non server
        if (session->getEditorCounter() == 1){
            timers.value(filename)->stop();
            // remove the timer
            timers.remove(filename);
        }

        // TODO: if editorCounter == 0 destroy the session
        // if editorCounter == 0 destroy the session
        // remove session from map active_sessions
        if(session->getEditorCounter() == 0){
            qDebug() << "List of active sessions: ";
            for(QString file: active_sessions.keys())
                qDebug() << file;

            qDebug() << "User " << username << " is the last user... Destroying the session and saving the file";

            // destroy the session
            active_sessions.erase(active_sessions.find(filename));

            // check the file is no more in the list of opened file
            qDebug() << "List of active sessions: ";
            for(QString file: active_sessions.keys())
                qDebug() << file;

            // saving the file
            int result = checkIfFileExists(filename.toStdString());
            if (result == -1) {
                message["body"] = "internal_error";
                sendMessage(message, active_socket);
                printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
                return false;
            }
            if (result == 0) {
                message["body"] = "not_existing_file";
                sendMessage(message, active_socket);
                printConsole("Trying to save file <i>" + filename + "</i>, but it doesn't exist anymore", true);
                return false;
            }

            QFile fo((QString::fromStdString(fs_root) + filename));
            fo.open(QIODevice::WriteOnly);

            QByteArray byteArrayBuffer;
            QDataStream outStream(&byteArrayBuffer, QIODevice::ReadWrite);
            outStream << session->getSymbols().values();
            if(fo.isOpen()){
                fo.write(byteArrayBuffer);
                fo.close();
            } else {
                message["body"] = "internal_error";
                sendMessage(message, active_socket);
                printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
                return false;
            }

            message["body"] = "ok";
            message["filename"] = data["filename"];
            sendMessage(message, active_socket);
            printConsole("<i>" + filename + "</i> correctly saved");
            return true;
        }
    } else {
        // session doesn't exist, display error message and nothing else
        if(idleConnectedUsers.contains(username)){
            qDebug() << "User is connected, can proceed";
            qDebug() << "The session doesn't exist, something went wrong";
        }

        printConsole("Sending error message to the client");
        // send error message to client
        QJsonObject message;
        message["body"] = "error_file";
        sendMessage(message, active_socket);
        printConsole("Client couldn't disconnect from session file: " + filename + "</i> for user <i>" + username + "</i>");
        return false;
    }

    // send message to client

    message["body"] = "ok";
    sendMessage(message, active_socket);
    printConsole("Client disconnected from session file: " + filename + "</i> for user <i>" + username + "</i>");
    return true;
}

bool Server::saveFile(QJsonObject &data, QTcpSocket *active_socket) {
    auto filename = data["filename"].toString();
    auto content = QByteArray::fromBase64(data["content"].toString().toLatin1());

    qDebug() << "Saving file with content " << content;

    QJsonObject message;

    int result = checkIfFileExists(filename.toStdString());
    if (result == -1) {
        message["header"] = "savefile";
        message["body"] = "internal_error";
        sendMessage(message, active_socket);
        printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
        return false;
    }
    if (result == 0) {
        message["header"] = "savefile";
        message["body"] = "not_existing_file";
        sendMessage(message, active_socket);
        printConsole("Trying to save file <i>" + filename + "</i>, but it doesn't exist anymore", true);
        return false;
    }

    QFile fo((QString::fromStdString(fs_root) + filename));
    fo.open(QIODevice::WriteOnly);
    if(fo.isOpen()){
        fo.write(content);
        fo.close();
    } else {
        message["header"] = "savefile";
        message["body"] = "internal_error";
        sendMessage(message, active_socket);
        printConsole("Internal server error while saving file <i>" + filename + "</i>", true);
        return false;
    }

    message["header"] = "savefile";
    message["body"] = "ok";
    message["filename"] = data["filename"];
    sendMessage(message, active_socket);
    printConsole("<i>" + filename + "</i> correctly saved");
    return true;
}

bool Server::receiveSymbol(QJsonObject &data, QTcpSocket *active_socket) {
    auto content = QByteArray::fromBase64(data["content"].toString().toLatin1());
    QString filename = data["filename"].toString().toLatin1();
    qDebug() << "filename is: " << filename;

    QDataStream inStream(&content, QIODevice::ReadOnly);
    Symbol sym;
    Session *session = this->active_sessions.value(filename);

    // acquire symbol
    inStream >> sym;
    qDebug() << "Received symbol " <<  "\"" << sym.getCharacter() << "\"" << "from" << data["editorId"];
    session->addSymbol(sym);
    data["header"] = "addSymbol";

    // now send symbol to all the other editors
    QString username = data["username"].toString();
    for(QString user : session->userEditorId.keys()){
        qDebug() << "Checking user" << user << " with editorId: " << session->userEditorId.value(user);
        if(user != username){
            qDebug() << "Sending symbol" << sym.getCharacter() << "to" << session->userEditorId.value(user);
            sendMessage(data, this->idleConnectedUsers.value(user));
        }
    }

    return true;
}

bool Server::deleteSymbol(QJsonObject &data, QTcpSocket *active_socket) {
    auto content = QByteArray::fromBase64(data["content"].toString().toLatin1());
    QString filename = data["filename"].toString().toLatin1();
    qDebug() << "filename is: " << filename;

    QDataStream inStream(&content, QIODevice::ReadOnly);
    QString symId;
    QString symPos;

    Session *session = this->active_sessions.value(filename);

    // acquire symbol
    inStream >> symId;
    qDebug() << "Deleting Symbol " << symId;
    data["header"] = "remSymbol";
    data["id"] = data["content"];
    QString stringPosition = session->getSymbols().value(symId).getPosition().getStringPosition();
    data["position"] = stringPosition;

    // now send deletion to all the other editors
    QString username = data["username"].toString();
    for(QString user : session->userEditorId.keys()){
        qDebug() << "Checking user" << user << " with editorId: " << session->userEditorId.value(user);
        if(user != username){
            qDebug() << "Sending deletion of" << session->getSymbols().value(symId).getCharacter() << "with frac pos"
            << stringPosition << "to" << session->userEditorId.values(user);
            sendMessage(data, this->idleConnectedUsers.value(user));
        }
    }
    session->removeSymbol(symId);

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

void Server::sendColors(QString filename){
    Session *session = this->active_sessions.value(filename);

    // TODO: mandare lista utente -> colore_posizione
    QJsonObject message;
    message["header"] = "colors";
    for (QString user : session->userMap.keys()){
        QTcpSocket *socket = this->idleConnectedUsers.value(user);

        for (QString user2 : session->userMap.keys()){
            if (user2 != user){
                QString color = session->userMap.value(user2).split("_")[0];
                QString position = session->userMap.value(user2).split("_")[1];

                // send user and color_position
                message["username"] = user2;
                message["color"] = color;
                message["position"] = position;
                if (socket != nullptr)
                    sendMessage(message, socket);
            }
        }
    }

    printConsole("Sending userMap for file: " + filename);
}

bool Server::updatePosition(QJsonObject &data, QTcpSocket *active_socket){
    // parse the json object
    QString username = data["username"].toString();
    QString filename = data["filename"].toString();
    QString position = data["position"].toString();
    qDebug() << "Updating position: " << position;

    Session *session = active_sessions.value(filename);

    if (username == ""){
        // something went wrong
        printConsole((QString &&) ("Something went wrong"));
        return false;
    }

    qDebug() << "Updating position of user:  " + username + " at position: " + position+ " in file: " + filename;

    // update position
    QString color_pos = session->userMap.value(username);
    color_pos = color_pos.split("_")[0] + "_" + position;
    session->userMap.insert(username, color_pos);
    return true;
}
