//
// Created by matteo on 25/03/20.
//

#include <fstream>
#include "LoginWindow.h"

LoginWindow::LoginWindow() {
	
	srv = new Server;
	regWin = new RegistrationWindow(srv);
	
	
	createGridGroupBox();
	
	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(gridGroupBox);
	setLayout(mainLayout);
	
	connect(loginButton, &QPushButton::released, this, &LoginWindow::checkPassword);
	connect(registrationLink, &QLabel::linkActivated, this, &LoginWindow::registerUser);
	
	setWindowTitle(tr("Login"));
}

void LoginWindow::createGridGroupBox() {
	gridGroupBox = new QGroupBox(tr("Login page"));
	auto layout = new QGridLayout;
	
	userLabel = new QLabel(tr("Username"));
	passwordLabel = new QLabel(tr("Password"));
	userEdit = new QLineEdit;
	passwordEdit = new QLineEdit;
	passwordEdit->setEchoMode(QLineEdit::Password);
	loginButton = new QPushButton(tr("Login"), this);
	registrationLink = new QLabel;
	registrationLink->setText("<a href=\"whatever\">Are you not registered yet?<\a>");
	registrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	
	layout->addWidget(userLabel, 0, 0);
	layout->addWidget(passwordLabel, 1, 0);
	layout->addWidget(userEdit, 0, 1);
	layout->addWidget(passwordEdit, 1, 1);
	layout->addWidget(loginButton, 1, 2);
	layout->addWidget(registrationLink, 2, 0, 1, 3);
	layout->setAlignment(registrationLink, Qt::AlignLeft);
	
	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 2);
	layout->setColumnStretch(2, 1);
	
	gridGroupBox->setLayout(layout);
}

void LoginWindow::checkPassword() {
	bool found = false;
	auto ul = srv->getUsernameList();
	auto pl = srv->getPasswordList();
	for (int i = 0; i < ul.size(); i++) {
		if (ul.at(i) == userEdit->text() & pl.at(i) == passwordEdit->text()) {
//			QMessageBox::information(this, tr("Correct"), tr("You are now logged in."));
			close();
			mainWin.setUsername(userEdit->text());
			mainWin.welcomeStatusBar();
			mainWin.show();
			found = true;
		}
	}
	if (!found) {
		QMessageBox::critical(this, tr("Error"), tr("Wrong username or password.\nTry again."));
	}
}

void LoginWindow::registerUser() {
	regWin->show();
}