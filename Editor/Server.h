//
// Created by matteo on 28/03/20.
//

#ifndef EDITOR_SERVER_H
#define EDITOR_SERVER_H

#include <QCoreApplication>

class Server: public QMainWindow {
	Q_OBJECT

private:
	QStringList usernameList;
	QStringList passwordList;

public:
	Server();
	QStringList getUsernameList();
	QStringList getPasswordList();
	void pushUsernameList(const QString& str);
	void pushPasswordList(const QString& str);
	void readUsernames();
	void readPasswords();

};


#endif //EDITOR_SERVER_H
