//
// Created by matteo on 28/03/20.
//

#include <QtWidgets>
#include "Server.h"

Server::Server() {
	readUsernames();
	readPasswords();
}

QStringList Server::getUsernameList() {
	return usernameList;
}

QStringList Server::getPasswordList() {
	return passwordList;
}

void Server::pushUsernameList(const QString& str) {
	usernameList.push_back(str);
}

void Server::pushPasswordList(const QString& str) {
	passwordList.push_back(str);
}

void Server::readUsernames() {
	QFile file("../LoginFiles/fileUsernames.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to get the registered users."));
		exit(EXIT_FAILURE);
	}
	
	QTextStream in(&file);
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		usernameList.append(line);
	}
	
	file.close();
}

void Server::readPasswords() {
	QFile file("../LoginFiles/filePasswords.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to get the users' passwords."));
		exit(EXIT_FAILURE);
	}
	
	QTextStream in(&file);
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		passwordList.append(line);
	}
	
	file.close();
}