//
// Created by matteo on 28/03/20.
//

#include <QtWidgets>
#include "RegistrationWindow.h"

RegistrationWindow::RegistrationWindow(Server* srv): srv(srv) {
	
	createGridGroupBox();
	
	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(gridGroupBox);
	setLayout(mainLayout);
	
	connect(registrationButton, &QPushButton::released, this, &RegistrationWindow::checkCredentials);
	
	setWindowTitle(tr("Registration"));
}

void RegistrationWindow::createGridGroupBox() {
	gridGroupBox = new QGroupBox(tr("Registration page"));
	auto layout = new QGridLayout;
	
	userLabel = new QLabel(tr("Username"));
	passwordLabel = new QLabel(tr("Password"));
	repeatPasswordLabel = new QLabel(tr("Repeat password"));
	userEdit = new QLineEdit;
	passwordEdit = new QLineEdit;
	passwordEdit->setEchoMode(QLineEdit::Password);
	repeatPasswordEdit = new QLineEdit;
	repeatPasswordEdit->setEchoMode(QLineEdit::Password);
	registrationButton = new QPushButton(tr("Register"), this);
	
	layout->addWidget(userLabel, 0, 0);
	layout->addWidget(passwordLabel, 1, 0);
	layout->addWidget(repeatPasswordLabel, 2, 0);
	layout->addWidget(userEdit, 0, 1);
	layout->addWidget(passwordEdit, 1, 1);
	layout->addWidget(repeatPasswordEdit, 2, 1);
	layout->addWidget(registrationButton, 3, 1);
	
	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 2);
	layout->setColumnStretch(2, 1);
	
	gridGroupBox->setLayout(layout);
}

void RegistrationWindow::checkCredentials() {
	for (auto& name: srv->getUsernameList()) {
		if (name == userEdit->text()) {
			QMessageBox::warning(this, tr("Retry"), tr("This username has already been taken.\nTry another one."));
			return;
		}
	}
	
	if (passwordEdit->text() != repeatPasswordEdit->text()) {
		QMessageBox::warning(this, tr("Retry"), tr("The two passwords are not equal.\nTry again."));
		return;
	}
	
	registerUser();
	close();
}

void RegistrationWindow::registerUser() {
	QFile fileUsernames("../LoginFiles/fileUsernames.txt");
	if (!fileUsernames.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to get the registered users."));
		exit(EXIT_FAILURE);
	}
	QFile filePasswords("../LoginFiles/filePasswords.txt");
	if (!filePasswords.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to get the users' passwords."));
		exit(EXIT_FAILURE);
	}
	QTextStream outUsername(&fileUsernames);
	outUsername << userEdit->text() << "\n";
	fileUsernames.close();
	
	QTextStream outPassword(&filePasswords);
	outPassword << passwordEdit->text() << "\n";
	filePasswords.close();
	
	srv->pushUsernameList(userEdit->text());
	srv->pushPasswordList(passwordEdit->text());
}