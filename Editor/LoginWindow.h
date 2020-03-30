//
// Created by matteo on 25/03/20.
//

#ifndef EDITOR_LOGINWINDOW_H
#define EDITOR_LOGINWINDOW_H

#include <QDialog>
#include <QtWidgets>
#include "TextEdit.h"
#include "RegistrationWindow.h"
#include "Server.h"

class LoginWindow: public QDialog
{
Q_OBJECT
friend class RegistrationWindow;

private:
	QGroupBox* gridGroupBox;
	QLabel* userLabel;
	QLabel* passwordLabel;
	QLineEdit* userEdit;
	QLineEdit* passwordEdit;
	QPushButton* loginButton;
	QLabel* registrationLink;
	
	TextEdit mainWin;
	RegistrationWindow* regWin;
	
	Server* srv;

	void createGridGroupBox();

public:
	LoginWindow();

private slots:
	void checkPassword();
	void registerUser();
};


#endif //EDITOR_LOGINWINDOW_H
