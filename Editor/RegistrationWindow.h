//
// Created by matteo on 28/03/20.
//

#ifndef EDITOR_REGISTRATIONWINDOW_H
#define EDITOR_REGISTRATIONWINDOW_H

#include "Server.h"

class RegistrationWindow: public QDialog {
	Q_OBJECT

private:
	QGroupBox* gridGroupBox;
	QLabel* userLabel;
	QLabel* passwordLabel;
	QLabel* repeatPasswordLabel;
	QLineEdit* userEdit;
	QLineEdit* passwordEdit;
	QLineEdit* repeatPasswordEdit;
	QPushButton* registrationButton;
	
	Server* srv;
	
	void createGridGroupBox();

public:
	explicit RegistrationWindow(Server* srv);

private slots:
	void checkCredentials();
	void registerUser();
};


#endif //EDITOR_REGISTRATIONWINDOW_H
