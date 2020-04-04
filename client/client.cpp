#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <string>
#include "client.h"
#include "ui_loginwindow.h"

Client::Client(QWidget *parent): QDialog(parent),tcpSocket(new QTcpSocket(this)),ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    statusBar = new QStatusBar(this);
    ui->verticalLayout->addWidget(statusBar);
    
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->LoginButton->setDefault(true);
    ui->LoginButton->setEnabled(false);
    ui->CancelButton->setEnabled(false);
	
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(ui->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
	connect(ui->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableLogButton);
    connect(ui->LoginButton, &QPushButton::released, this, &Client::requestLogin);
    connect(ui->CancelButton, &QPushButton::released, this, &Client::cancelRequestLogin);
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
	
	    ui->LoginButton->setEnabled(false);
	    statusBar->showMessage(tr("Opening network session..."));
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
//    auto host = "127.0.0.1";
    int port = 4848;
    ui->LoginButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
    ui->UsernameEdit->setReadOnly(true);
    ui->PasswordEdit->setReadOnly(true);
    ui->UsernameEdit->clearFocus();
    ui->CancelButton->setEnabled(true);
    statusBar->showMessage(tr("Trying to connect..."));
    qDebug() << "Trying to connect...";
}

void Client::cancelRequestLogin() {
	ui->UsernameEdit->setReadOnly(false);
	ui->PasswordEdit->setReadOnly(false);
	ui->LoginButton->setEnabled(true);
	ui->CancelButton->setEnabled(false);
	statusBar->showMessage("");
	qDebug() << "";
}

void Client::readResponse()
{
	statusBar->showMessage(tr("Reading the response..."));
	qDebug() << "Reading the response...";
    in.startTransaction();

    QString response;
    in >> response;

    if (!in.commitTransaction()) {
    	return;
    }
    
	statusBar->showMessage(response);
    qDebug().noquote() << response;
    ui->LoginButton->setEnabled(true);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
	cancelRequestLogin();
	
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
    ui->LoginButton->setEnabled((!networkSession || networkSession->isOpen()) && !ui->UsernameEdit->text().isEmpty() && !ui->PasswordEdit->text().isEmpty());

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
	
    statusBar->showMessage(tr("Connected to the server."));
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
        
        out << QString(ui->UsernameEdit->text() + "_" + ui->PasswordEdit->text());
        
        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message."));
        }
        tcpSocket->flush();
    }
	
    statusBar->showMessage(tr("Credential sent, waiting for reply..."));
    qDebug() << "Credential sent, waiting for reply...";
}

