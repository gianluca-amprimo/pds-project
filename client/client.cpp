#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <string>
#include "client.h"
#include "ui_loginwindow.h"
#include "ui_registrationwindow.h"
#include "ui_cancellationwindow.h"

Client::Client(QWidget *parent): QDialog(parent), tcpSocket(new QTcpSocket(this)), uiLog(new Ui::LoginWindow)
{
    uiLog->setupUi(this);
    logStatusBar = new QStatusBar(this);
    uiLog->verticalLayout->addWidget(logStatusBar);
	uiLog->RegistrationLink->setText("<a href=\"whatever\">Are you registered yet?</a>");
	uiLog->CancellationLink->setText("<a href=\"whatever\">Do you want to cancel your account?</a>");
	
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(uiLog->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
	connect(uiLog->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
    connect(uiLog->LoginButton, &QPushButton::released, this, &Client::requestLogin);
    connect(uiLog->RegistrationLink, &QLabel::linkActivated, this, &Client::openRegistrationWindow);
	connect(uiLog->CancellationLink, &QLabel::linkActivated, this, &Client::openCancellationWindow);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readResponse);
    connect(tcpSocket, &QAbstractSocket::connected  ,this, &Client::sendCredentials);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);
    
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
	    // Get saved network configuration
	    QSettings settings(QSettings::UserScope, QLatin1String("PdsProject"));
	    settings.beginGroup(QLatin1String("QtNetwork"));
	    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
	    settings.endGroup();
	
	    // If the saved network configuration is not currently discovered use the system default
	    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
	    if ((config.state() & QNetworkConfiguration::Discovered) !=
	        QNetworkConfiguration::Discovered) {
		    config = manager.defaultConfiguration();
	    }
	
	    networkSession = new QNetworkSession(config, this);
	    connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);
	
	    uiLog->LoginButton->setEnabled(false);
	    logStatusBar->showMessage(tr("Opening network session..."));
	    qDebug() << "Opening network session...";
	    networkSession->open();
    }
}

void Client::requestLogin()
{
#ifndef LOCALHOST
	auto host = "128.0.0.1";
#else
	auto host = QHostAddress(QHostAddress::LocalHost).toString();
#endif
    int port = 4848;
    uiLog->LoginButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
    uiLog->UsernameEdit->setReadOnly(true);
    uiLog->PasswordEdit->setReadOnly(true);
    uiLog->UsernameEdit->clearFocus();
    uiLog->RegistrationLink->setTextInteractionFlags(Qt::NoTextInteraction);
	uiLog->CancellationLink->setTextInteractionFlags(Qt::NoTextInteraction);
	uiLog->RegistrationLink->setCursor(QCursor(Qt::ArrowCursor));
	uiLog->CancellationLink->setCursor(QCursor(Qt::ArrowCursor));
    logStatusBar->showMessage(tr("Trying to connect..."));
    qDebug() << "Trying to connect...";
}

void Client::readResponse()
{
	logStatusBar->showMessage(tr("Reading the response..."));
	qDebug() << "Reading the response...";
    in.startTransaction();

    QString response;
    in >> response;

    if (!in.commitTransaction()) {
    	return;
    }
    
	logStatusBar->showMessage(response);
    qDebug().noquote() << response;
    
    if (response.toStdString() == "Success") {
	    // TODO: aprire l'editor
	    this->close();
    } else {
    	uiLog->LoginButton->setEnabled(true);
	    uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	    uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	    uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
	    uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
    }
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
	uiLog->UsernameEdit->setReadOnly(false);
	uiLog->PasswordEdit->setReadOnly(false);
	uiLog->LoginButton->setEnabled(true);
	uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
	uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
	logStatusBar->showMessage("");
	qDebug() << "";
	
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("PdS Client"),tr("The host was not found. Please check the host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Pds Client"),tr("The connection was refused by the peer. Make sure the PdS server is running and check that the host name and port settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("PdS Client"),tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
    }
}

void Client::enableLogButton()
{
    uiLog->LoginButton->setEnabled((!networkSession || networkSession->isOpen()) && !uiLog->UsernameEdit->text().isEmpty() && !uiLog->PasswordEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    enableLogButton();
}

void Client::sendCredentials(){
	
    logStatusBar->showMessage(tr("Connected to the server."));
    qDebug() << "Connected to the server.";

    if (tcpSocket != nullptr) {
        if (!tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        
        out << QString(uiLog->UsernameEdit->text() + "_" + uiLog->PasswordEdit->text());
        
        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message."));
        }
        tcpSocket->flush();
    }
	
    logStatusBar->showMessage(tr("Credential sent, waiting for reply..."));
    qDebug() << "Credential sent, waiting for reply...";
}

void Client::openRegistrationWindow() {
	this->setVisible(false);
	
	uiReg = new Ui::RegistrationWindow;
	RegWin = new QDialog;
	uiReg->setupUi(RegWin);
	regStatusBar = new QStatusBar(RegWin);
	uiReg->verticalLayout->addWidget(regStatusBar);
	
	connect(uiReg->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RepeatPasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RegisterButton, &QPushButton::released, this, &Client::requestRegistration);
	connect(RegWin, &QDialog::finished, this, &Client::reactivateLoginWindow);
	
	RegWin->show();
}

void Client::enableRegButton() {
	if (!uiReg->UsernameEdit->text().isEmpty() & !uiReg->PasswordEdit->text().isEmpty()) {
		if (uiReg->PasswordEdit->text() != uiReg->RepeatPasswordEdit->text()) {
			if (uiReg->RegisterButton->isEnabled()) {
				uiReg->RegisterButton->setEnabled(false);
			}
			if (regStatusBar->currentMessage() != QString(tr("The two passwords do not coincide!"))) {
				regStatusBar->showMessage(tr("The two passwords do not coincide!"));
				qDebug() << "The two passwords do not coincide!";
			}
		} else {
			regStatusBar->showMessage(tr(""));
			uiReg->RegisterButton->setEnabled(true);
		}
	}
}

void Client::requestRegistration() {
	regStatusBar->showMessage(tr("Checking database..."));
	qDebug() << "Checking database...";
	
	// TODO: connettere al server per la registrazione di username e password
	
	auto success = true;        // risultato dell'operazione
	
	if (success) {
		logStatusBar->showMessage(tr("Successful registration."), 3000);
		qDebug() << "Successful registration.";
		RegWin->close();
		reactivateLoginWindow();
	} else {
		regStatusBar->showMessage(tr("Registration failed"));
		qDebug() << "Registration failed";
	}
}

void Client::reactivateLoginWindow() {
	uiLog->LoginButton->setEnabled(!(uiLog->UsernameEdit->text().isEmpty() | uiLog->PasswordEdit->text().isEmpty()));
	if (uiLog->RegistrationLink->hasFocus()) {
		uiLog->RegistrationLink->clearFocus();
	}
	if (uiLog->CancellationLink->hasFocus()) {
		uiLog->CancellationLink->clearFocus();
	}
	this->setVisible(true);
}

void Client::openCancellationWindow() {
	this->setVisible(false);
	
	uiCanc = new Ui::CancellationWindow;
	CancWin = new QDialog;
	uiCanc->setupUi(CancWin);
	cancStatusBar = new QStatusBar(CancWin);
	uiCanc->verticalLayout->addWidget(cancStatusBar);
	
	connect(uiCanc->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableDelButton);
	connect(uiCanc->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableDelButton);
	connect(uiCanc->DeleteButton, &QPushButton::released, this, &Client::requestDeletion);
	connect(CancWin, &QDialog::finished, this, &Client::reactivateLoginWindow);
	
	CancWin->show();
}

void Client::enableDelButton() {
	uiCanc->DeleteButton->setEnabled(!uiCanc->UsernameEdit->text().isEmpty() & !uiCanc->PasswordEdit->text().isEmpty());
}

void Client::requestDeletion() {
	cancStatusBar->showMessage(tr("Deleting account..."));
	qDebug() << "Deleting account...";
	
	// TODO: connettere al server per la registrazione di username e password
	
	auto success = true;            // risultato dell'operazione
	
	if (success) {
		logStatusBar->showMessage(tr("Successful cancellation."), 3000);
		CancWin->close();
		reactivateLoginWindow();
	} else {
		cancStatusBar->showMessage(tr("Cancellation failed"));
		qDebug() << "Cancellation failed";
	}
}
