#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <sstream>
#include <string>
#include "client.h"
#include "ui_loginwindow.h"
#include "ui_registrationwindow.h"
#include "ui_cancellationwindow.h"
#include "ui_filechoicewindow.h"

Client::Client(QWidget *parent): QDialog(parent), tcpSocket(new QTcpSocket(this)), uiLog(new Ui::LoginWindow)
{
    uiLog->setupUi(this);
    logStatusBar = new QStatusBar(this);
    uiLog->verticalLayout->addWidget(logStatusBar);
	uiLog->RegistrationLink->setText("<a href=\"whatever\">Click here to register</a>");
	uiLog->CancellationLink->setText("<a href=\"whatever\">Click here to cancel your account</a>");
	
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(uiLog->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
	connect(uiLog->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
    connect(uiLog->LoginButton, &QPushButton::released, this, &Client::sendCredentials);
    connect(uiLog->RegistrationLink, &QLabel::linkActivated, this, &Client::openRegistrationWindow);
	connect(uiLog->CancellationLink, &QLabel::linkActivated, this, &Client::openCancellationWindow);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readResponse);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);
    Client::requestConnection();
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

void Client::requestConnection()
{
    qDebug()<< "requesting connection";
#ifndef LOCALHOST
	auto host = "128.0.0.1";
#else
	auto host = QHostAddress(QHostAddress::LocalHost).toString();
#endif
    int port = 4848;
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
    //TODO: do something if connection to server fails
}

void Client::readResponse()
{
	logStatusBar->showMessage(tr("Reading the response..."));
	qDebug() << "Reading the response...";
    in.startTransaction();

//read the Json message received from client, from header understand what to do
    in.startTransaction();

    QByteArray jSmessage;
    std::string header;
    std::string result;
    QJsonObject jSobject;

    in >> jSmessage;
    QJsonParseError parseError;
    // we try to create a json document with the data we received
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jSmessage, &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        // if the data was indeed valid JSON
        if (jsonDoc.isObject()){
            jSobject=jsonDoc.object();
            header=jSobject["header"].toString().toStdString();
            result=jSobject["body"].toString().toStdString();
            qDebug()<<jSobject["header"].toString()<<" "<<jSobject["body"].toString();
        }
        else{
            //TODO: error with the json do something (implement function for generic error message to client)
        }

    }
    else {
        //TODO: error with the json do something (implement function for generic error message to client)
    }


    if (!in.commitTransaction()) {
    	return;
    }
    
	logStatusBar->showMessage(QString::fromStdString(header+':'+result));


    if(header=="log") {
        if (result == "ok") {
	        qDebug() << "Successful login.";
	        QString un(uiLog->UsernameEdit->text());
            openFileChoiceWindow(un);
            this->close();
        }
        if(result=="unreg"){
            QMessageBox::information(this, tr("PdS Server"), tr("Utente non trovato! Ricontrolla user e password"));
            uiLog->LoginButton->setEnabled(true);
	        uiLog->UsernameEdit->setReadOnly(false);
	        uiLog->PasswordEdit->setReadOnly(false);
            uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
            uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
            uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
            uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
        }
        if(result=="fail"){
            QMessageBox::information(this, tr("PdS Server"), tr("Errore nel server, riprovare più tardi"));
            uiLog->LoginButton->setEnabled(true);
            uiLog->UsernameEdit->setReadOnly(false);
            uiLog->PasswordEdit->setReadOnly(false);
            uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
            uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
            uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
            uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
        }
    }
    if(header=="reg") {
        if (result=="ok") {
            qDebug() << "Successful registration.";
	        QString un(uiReg->UsernameEdit->text());
            openFileChoiceWindow(un);
            RegWin->close();
        }
        if (result=="sfail"){
            QMessageBox::information(this, tr("PdS Server"), tr("Registrazione fallita per problemi al server, ritentare"));
            uiReg->NameEdit->setReadOnly(false);
            uiReg->SurnameEdit->setReadOnly(false);
            uiReg->UsernameEdit->setReadOnly(false);
            uiReg->PasswordEdit->setReadOnly(false);
	        uiReg->RepeatPasswordEdit->setReadOnly(false);
            regStatusBar->showMessage(tr("Registration failed"));
            qDebug() << "Registration failed";
        }
        if(result=="alreadyreg"){
            QMessageBox::information(this, tr("PdS Server"), tr("Nome utente già registrato! Inserisci un altro user o esegui il login"));
            uiReg->NameEdit->setReadOnly(false);
            uiReg->SurnameEdit->setReadOnly(false);
            uiReg->UsernameEdit->setReadOnly(false);
            uiReg->PasswordEdit->setReadOnly(false);
            regStatusBar->showMessage(tr("Registration failed"));
            qDebug() << "Registration failed";
        }

    }
    if(header=="canc"){
        if (result=="ok") {
            logStatusBar->showMessage(tr("Successful cancellation."), 3000);
            CancWin->close();
            reactivateLoginWindow();
        }
        if( result=="notpres"){
            cancStatusBar->showMessage(tr("Attenzione! L’utente da cancellare non esiste, riprovare"));
            qDebug() << "User doesnt exist";
        }
        if( result=="fail"){
            cancStatusBar->showMessage(tr("Cancellation failed"));
            qDebug() << "Cancellation failed";
        }
    }
    if(header=="flist");

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

     uiLog->LoginButton->setEnabled(false);
     uiLog->UsernameEdit->setReadOnly(true);
     uiLog->PasswordEdit->setReadOnly(true);
     uiLog->UsernameEdit->clearFocus();
     uiLog->RegistrationLink->setTextInteractionFlags(Qt::NoTextInteraction);
     uiLog->CancellationLink->setTextInteractionFlags(Qt::NoTextInteraction);
     uiLog->RegistrationLink->setCursor(QCursor(Qt::ArrowCursor));
     uiLog->CancellationLink->setCursor(QCursor(Qt::ArrowCursor));
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
        QJsonObject message;
        message["header"] = "log";
        message["username"] = uiLog->UsernameEdit->text();
        message["password"] = uiLog->PasswordEdit->text();
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();
        
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
	
	connect(uiReg->NameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->SurnameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RepeatPasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RegisterButton, &QPushButton::released, this, &Client::requestRegistration);
//	connect(RegWin, &QDialog::finished, this, &Client::reactivateLoginWindow);
	
    RegWin->show();
}

void Client::enableRegButton() {
	if (!uiReg->NameEdit->text().isEmpty() & !uiReg->SurnameEdit->text().isEmpty() & !uiReg->UsernameEdit->text().isEmpty() & !uiReg->PasswordEdit->text().isEmpty()) {
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
	} else if (uiReg->PasswordEdit->text().isEmpty() & uiReg->RepeatPasswordEdit->text().isEmpty() & regStatusBar->currentMessage() != QString(tr(""))) {
		regStatusBar->showMessage(tr(""));
	}
}

void Client::requestRegistration() {
	uiReg->NameEdit->setReadOnly(true);
	uiReg->SurnameEdit->setReadOnly(true);
	uiReg->UsernameEdit->setReadOnly(true);
	uiReg->PasswordEdit->setReadOnly(true);
	uiReg->RepeatPasswordEdit->setReadOnly(true);
	
	regStatusBar->showMessage(tr("Checking database..."));
	qDebug() << "Checking database...";

    if (tcpSocket != nullptr) {
        if (!tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    QJsonObject message;
    message["header"] = "reg";
    message["username"] = uiReg->UsernameEdit->text();
    message["password"] = uiReg->PasswordEdit->text();
    message["name"] = uiReg->NameEdit->text();
    message["surname"] = uiReg->SurnameEdit->text();

    // send the JSON using QDataStream
    out << QJsonDocument(message).toJson();

    if (!tcpSocket->write(block)) {
        QMessageBox::information(this, tr("PdS Server"), tr("Could not send message."));
    }
    tcpSocket->flush();

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
        QJsonObject message;
        message["header"] = "canc";
        message["username"] = uiCanc->UsernameEdit->text();
        message["password"] = uiCanc->PasswordEdit->text();
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message."));
        }
        tcpSocket->flush();
    }

    logStatusBar->showMessage(tr("Credential sent, waiting for reply..."));
    qDebug() << "Credential sent, waiting for reply...";

}

void Client::openFileChoiceWindow(QString username) {
	uiChoice = new Ui::FileChoiceWindow;
	ChoiceWin = new QDialog;
	uiChoice->setupUi(ChoiceWin);
	uiChoice->OpenMenu->completer()->setCompletionMode(QCompleter::PopupCompletion);
	uiChoice->OpenMenu->completer()->setFilterMode(Qt::MatchContains);
	uiChoice->OpenMenu->installEventFilter(this);
	auto cbModel = new QStringListModel;
	uiChoice->OpenMenu->setModel(cbModel);
	
	// TODO: mettere nome invece che username
	uiChoice->WelcomeLabel->setText(tr("Welcome back,\n%1!").arg(username));
	
	QStringList fileList;
	fileList << "File1.txt" << "File2.txt" <<  "File3.txt" << "File4.txt" << "File5.txt" << "File6.txt" << "File7.txt" << "File8.txt" << "File9.txt" << "File10.txt" << "File11.txt" << "File12.txt" << "Prova" << "Ciao";
	for (auto &file: fileList) {
		uiChoice->OpenMenu->addItem(file);
	}
	uiChoice->OpenMenu->setCurrentText("");                 // Questo deve stare dopo il caricamento della lista
	
	connect(uiChoice->NewButton, &QPushButton::released, this, &Client::openNewFile);
	connect(uiChoice->OpenButton, &QPushButton::released, this, &Client::openExistingFile);
	connect(uiChoice->OpenMenu->lineEdit(), &QLineEdit::returnPressed, this, &Client::openExistingFile);
	// TODO: connettere button impostazioni
	
	ChoiceWin->show();
}

void Client::openNewFile() {
	// TODO: aprire nuovo file
	qDebug() << "Opening new file";
	ChoiceWin->close();
}

void Client::openExistingFile() {
	// TODO: aprire file selezionato
	if (uiChoice->OpenMenu->findText(uiChoice->OpenMenu->currentText()) == -1) {
		auto mes = QMessageBox::information(ChoiceWin, tr("Error"), tr("The file does not exist."), QMessageBox::Ok);
		qDebug() << "The file does not exist.";
		return;
	}
	qDebug() << "Opening selected file...";
	ChoiceWin->close();
}

bool Client::eventFilter(QObject *object, QEvent *event) {
	if (event->type() == QEvent::FocusIn) {
		if (object == uiChoice->OpenMenu) {
			uiChoice->NewButton->setDefault(false);
		}
	}
}
