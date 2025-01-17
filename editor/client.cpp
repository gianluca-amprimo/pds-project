#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <regex>
#include "client.h"
#include "ui_LoginWin.h"
#include "ui_DeleteAccountWin.h"
#include "ui_WelcomeWin.h"
#include "ui_SignupWin.h"
#include "ui_ProfileSettingsWin.h"
#include "ui_NewFile.h"
#include "ui_WelcomeWinClone.h"
static QString defaultPicture(":/misc/themes/material/user.png");

Client::Client(QWidget *parent): QDialog(parent), tcpSocket(new QTcpSocket(this)) {
	uiLog = std::make_shared<Ui::LoginWin>();
    uiLog->setupUi(this);
    this->setWindowTitle("PiDiEsse - Login");
    logStatusBar = std::make_shared<QStatusBar> (this);
    uiLog->verticalLayout->addWidget(logStatusBar.get());


	logHidePassword = uiLog->PasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	logPasswordButton = qobject_cast<QToolButton *>(logHidePassword->associatedWidgets().last());
	logPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(logPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiLog->PasswordEdit); });
	connect(logPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiLog->PasswordEdit); });

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
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::requestConnection);

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
	    logStatusBar->showMessage(tr("Opening network session..."), 3000);

#if DEBUG
	    qDebug() << "Opening network session...";
#endif
	    networkSession->open();
    }
	uiLog->UsernameEdit->setFocus();            // questo deve stare ultimo
}

void Client::requestConnection()
{
#if DEBUG
    qDebug()<< "requesting connection";
#endif

#ifndef LOCALHOST
    // define whatever IP address the server is running on
	auto host = "128.0.0.1";
#else
	auto host = QHostAddress(QHostAddress::LocalHost).toString();
#endif
    int port = 4848;
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);

}

void Client::readResponse()
{
	logStatusBar->showMessage(tr("Reading the response..."));

#if DEBUG
    qDebug() << "Reading the response...";
#endif

    QByteArray jSmessage;
    std::string header;
    std::string result;
    QJsonObject jSobject;
    QJsonArray jSarray;
    QPixmap propic;
    QJsonParseError parseError;

#if DEBUG
    qDebug() << "Going to read" << this->tcpSocket->bytesAvailable() << "bytes";
#endif

    // read the Json message received from client, from header understand what to do
    while(!in.atEnd()) {

        in.startTransaction();
        in >> jSmessage;

#if DEBUG
        qDebug() << "message length is" << jSmessage.length();
#endif

        if (!in.commitTransaction()) {
            return;
        }
    }
    // we try to create a json document with the data we received
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jSmessage, &parseError);
    if (parseError.error == QJsonParseError::NoError) {
                                                  // if the data was indeed valid JSON
                                                  if (jsonDoc.isObject()) {
                                                  jSobject = jsonDoc.object();
                                                  header = jSobject["header"].toString().toStdString();
                                                  result = jSobject["body"].toString().toStdString();
                                                  } else if (jsonDoc.isArray()) {
                                                  jSarray = jsonDoc.array();
                                                  header = jSarray[0].toObject()["header"].toString().toStdString();
                                                  } else {
                                                  QMessageBox::information(this, tr("PdS Server"), tr("Generic Server error.\nTry again later"));
                                                  QApplication::quit();
                                                  }

                                                  } else {
                                                             QString filename = "responseLog.json";
                                                             QFile file(filename);
                                                             QTextStream fileStream(&file);
                                                             #if DEBUG
                                                                qDebug() << "Error in response of type" << parseError.error << "at offset" << parseError.offset;
                                                                qDebug() << "Logging Json response to " << filename;
                                                             #endif

                                                             if (file.open(QIODevice::WriteOnly)) {
                                                             fileStream << QString::fromStdString(jsonDoc.toJson().toStdString());
                                                             file.close();
                                                             }
                                                             if (jsonDoc.isEmpty()) {
                                                             QMessageBox::information(this, tr("PdS Server"), tr("Server response is empty"));
                                                             } else {
                                                             QMessageBox::information(this, tr("PdS Server"), tr("Error in the server response"));

                                                             }
                                                             QApplication::quit();
                                                             }


#if DEBUG
    qDebug().noquote() << QString::fromStdString(header + ": " + result);
#endif

    if (header == "error") {
                       QMessageBox::information(this, tr("PdS Server"), tr("Generic Server error.\nTry again later"));
                       QApplication::quit();
                       }
    if (header == "log") {
                     if (result == "ok") {
                     setFileList(jSobject);
                     //create the associated user
                     User u(uiLog->UsernameEdit->text(), pixmapFrom(jSobject["propic"]), jSobject["name"].toString(),
                     jSobject["surname"].toString());
                     qDebug() << u.getName() << " " << u.getSurname();
                     loggedUser = std::make_shared<User>(u);
                     openWelcomeWin2(false);
                     this->close();
                     }
                     if (result == "unreg") {
                     QMessageBox::information(this, tr("PdS Server"),
                     tr("User not found.\nCheck again username and password."));
                     logStatusBar->showMessage(tr("User not found."), 3000);
                     uiLog->LoginButton->setEnabled(true);
                     uiLog->UsernameEdit->setReadOnly(false);
                     uiLog->PasswordEdit->setReadOnly(false);
                     uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
                     uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
                     uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
                     uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
                     QWidget::setTabOrder(uiLog->UsernameEdit, uiLog->PasswordEdit);
                     QWidget::setTabOrder(uiLog->PasswordEdit, uiLog->LoginButton);
                     }
                     if (result == "fail") {
                     QMessageBox::information(this, tr("PdS Server"), tr("Server error.\nTry again later."));
                     logStatusBar->showMessage(tr("Server error."), 3000);
                     uiLog->LoginButton->setEnabled(true);
                     uiLog->UsernameEdit->setReadOnly(false);
                     uiLog->PasswordEdit->setReadOnly(false);
                     uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
                     uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
                     uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
                     uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
                     QWidget::setTabOrder(uiLog->UsernameEdit, uiLog->PasswordEdit);
                     QWidget::setTabOrder(uiLog->PasswordEdit, uiLog->LoginButton);
                     }
                     }

    if (header == "reg") {
                     if (result == "ok") {
                     User u(uiReg->UsernameEdit->text(), pixmapFrom(jSobject["propic"]), jSobject["name"].toString(),
                     jSobject["surname"].toString());

                     loggedUser = std::make_shared<User>(u);
                     setFileList(jSobject);
                     openWelcomeWin2(true);
                     RegWin->close();
                     this->close();
                     }
                     if (result == "fail") {
                     QMessageBox::information(this, tr("PdS Server"), tr("Server error.\nTry again later."));
                     regStatusBar->showMessage(tr("Server error."), 3000);
                     uiReg->NameEdit->setReadOnly(false);
                     uiReg->SurnameEdit->setReadOnly(false);
                     uiReg->UsernameEdit->setReadOnly(false);
                     uiReg->PasswordEdit->setReadOnly(false);
                     uiReg->RepeatPasswordEdit->setReadOnly(false);
                     }
                     if (result == "alreadyreg") {
                     QMessageBox::information(this, tr("PdS Server"),
                     tr("This username is already taken.\nTry another one."));
                     regStatusBar->showMessage(tr("Username already taken."), 3000);
                     uiReg->NameEdit->setReadOnly(false);
                     uiReg->SurnameEdit->setReadOnly(false);
                     uiReg->UsernameEdit->setReadOnly(false);
                     uiReg->PasswordEdit->setReadOnly(false);
                     uiReg->RepeatPasswordEdit->setReadOnly(false);
                     }

                     }

    if (header == "canc") {
                      if (result == "ok") {
                      logStatusBar->showMessage(tr("Successful cancellation."), 3000);
                      CancWin->close();
                      reactivateLoginWindow();
                      }
                      if (result == "notpres") {
                      QMessageBox::information(this, tr("PdS Server"),
                      tr("User not found.\nCheck again the username and the password."));
                      cancStatusBar->showMessage(tr("User not found or wrong password."), 3000);
                      uiCanc->UsernameEdit->setReadOnly(false);
                      uiCanc->PasswordEdit->setReadOnly(false);
                      uiCanc->DeleteButton->setEnabled(true);
                      }
                      if (result == "fail") {
                      QMessageBox::information(this, tr("PdS Server"),
                      tr("Cancellation failed.\nCheck again username and password."));
                      logStatusBar->showMessage(tr("Cancellation failed."), 3000);
                      uiCanc->UsernameEdit->setReadOnly(false);
                      uiCanc->PasswordEdit->setReadOnly(false);
                      uiCanc->DeleteButton->setEnabled(true);
                      }
                      }

    if (header == "upd") {
                     if (result == "ok") {
                     User u(uiSett->UsernameEdit->text(), pixmapFrom(jSobject["propic"]), jSobject["name"].toString(),
                     jSobject["surname"].toString());

                     loggedUser = std::make_shared<User>(u);
                     avail_file.clear();
                     setFileList(jSobject);
                     SettWin->close();
                     uiChoice->ProfilePicture->setPixmap(circularPixmap(const_cast<QPixmap &&>(loggedUser->getPropic())));
                     ChoiceWin->setVisible(true);
                     }
                     if (result == "fail") {
                     QMessageBox::information(this, tr("PdS Server"), tr("Server error.\nTry again later."));
                     settStatusBar->showMessage(tr("Server error."), 3000);
                     uiSett->CurrentPasswordEdit->setReadOnly(false);
                     uiSett->NewPasswordEdit->setReadOnly(false);
                     uiSett->DeletePictureButton->setEnabled(true);
                     uiSett->ProfilePictureButton->setEnabled(true);
                     uiSett->UndoButton->setEnabled(true);
                     }
                     if (result == "wrongpass") {
                     QMessageBox::information(this, tr("PdS Server"), tr("The password is wrong.\nTry again."));
                     settStatusBar->showMessage(tr("Wrong password."), 3000);
                     uiSett->CurrentPasswordEdit->setReadOnly(false);
                     uiSett->NewPasswordEdit->setReadOnly(false);
                     uiSett->DeletePictureButton->setEnabled(true);
                     uiSett->ProfilePictureButton->setEnabled(true);
                     uiSett->UndoButton->setEnabled(true);
                     }
                     }

    if (header == "refr") {
        if (result == "ok") {
            setFileList(jSobject);

            // reset list of files
            uiChoice->OpenTable->clear();
            uiChoice->OpenTable->setRowCount(avail_file.size());
            uiChoice->OpenTable->setColumnCount(1);

            QStringList fileList;
            for (auto s: avail_file) {
                fileList += s;
            }
            int i = 0;
            for (auto &file: fileList) {
                QTableWidgetItem *item = new QTableWidgetItem(file);
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                uiChoice->OpenTable->setItem(i, 0, item);
                i++;
            }
        }
    }

    if (header == "newfile") {
                         if (result == "internal_error") {
                         QMessageBox::information(this, tr("PiDiEsse [client]"),
                         tr("Internal server error while creating the file.\nTry again later."));
                         } else if (result == "existing_file") {
                         QMessageBox::information(this, tr("PiDiEsse [client]"),
                         tr("You have just tried to create a new file, but it already exists\nTry with a new name."));
                         }
                         }

    if (header == "openfile") {
                          if (result == "new_session" || result == "existing_session") {
                          auto content = QByteArray::fromBase64(jSobject["content"].toString().toLatin1());
                          QDataStream contentStream(&content, QIODevice::ReadOnly);

                          auto filename = jSobject["filename"].toString();
                          qDebug() << filename;

                          this->mainEditor = new MainEditor(this, jSobject["editorId"].toString(), filename,
                          this->tcpSocket, &contentStream, loggedUsername);
                          mainEditor->show();
                          ChoiceWin->setVisible(false);
                          } else if (result == "internal_error") {
                          QMessageBox::information(this, tr("PiDiEsse [client]"),
                          tr("Internal server error while opening the file.\nTry again later."));
                          } else if (result == "not_existing_file") {
                          QMessageBox::information(this, tr("PiDiEsse [client]"),
                          tr("The file doesn't exist anymore.\nTry to update the list of files."));
                          }
                          }

    if (header == "savefile") {
                          if (result == "ok") {
                          mainEditor->getUi()->statusBar->showMessage(tr("File saved correctly."), 5000);
                          }
                          if (result == "internal_error") {
                          mainEditor->getUi()->statusBar->showMessage(
                          tr("Internal server error while creating the file. Try again later."), 5000);
                          } else if (result == "not_existing_file") {
                          mainEditor->getUi()->statusBar->showMessage(
                          tr("The file doesn't exist anymore. Try to create a new file."), 5000);
                          }
                          }

    if (header == "shareFile") {
                           if (result == "internal_error")
                           mainEditor->getUi()->statusBar->showMessage(
                           tr("Internal server error while creating the file. Try again later."), 5000);
                           if (result == "non_existing_file_or_user")
                           QMessageBox::information(ChoiceWin.get(), tr("PdS Server"),
                           tr("File or user not recognized. Try again."), QMessageBox::Ok);
                           if (result == "already_granted")
                           QMessageBox::information(ChoiceWin.get(), tr("PdS Server"), tr("You already have access to this file!"),
                           QMessageBox::Ok);
                           }

    if (header == "addSymbol") {
                           this->mainEditor->receiveSymbol(jSobject["content"], jSobject["username"]);
                           }

    if (header == "remSymbol") {
                           this->mainEditor->receiveDeletion(jSobject["id"], jSobject["position"], jSobject["username"]);
                           }

    if (header == "colors") {
                        this->mainEditor->colors(jSobject["username"].toString(), jSobject["color"].toString());
                        }
    if (header == "add1Symbol") {
                            this->mainEditor->receiveSymbol(jSobject["symbol"], jSobject["username"]);
                            }
    if (header == "delete1Symbol") {
                               this->mainEditor->receiveDeletion(jSobject["id"], jSobject["position"], jSobject["username"]);
                               }
    if (header == "deleteBatchSymbol") {
                                   this->mainEditor->receiveBatchDeletion(jSobject["idsAndPositions"], jSobject["username"]);
                                   }
    if (header == "addBatchSymbol") {
                                this->mainEditor->receiveBatchSymbol(jSarray);
                                }
    if (header == "changeSymbolFormat"){
        this->mainEditor->receiveFormatChanged(jSobject["idsAndPositions"], jSobject["format"]);
    }
}

const std::shared_ptr<QDialog> &Client::getChoiceWin() const {
    return ChoiceWin;
}

void Client::displayError(QAbstractSocket::SocketError socketError) {
	uiLog->UsernameEdit->setReadOnly(false);
	uiLog->PasswordEdit->setReadOnly(false);
	uiLog->LoginButton->setEnabled(true);
	uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
	uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
	logStatusBar->clearMessage();
	
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            QMessageBox::information(this, tr("PdS Client"),tr("Server closed connection. Relaunch the program in a few minutes"));
#if DEBUG
            qDebug() << "The server has disconnected. Client will close, try to launch again in few seconds.";
#endif
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("PdS Client"),tr("The host was not found.\nPlease check the host name and port settings."));
#if DEBUG
            qDebug() << "The host was not found. Please check the host name and port settings.";
#endif
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Pds Client"),tr("The connection was refused by the peer.\nMake sure the PdS server is running and check that the host name and port settings are correct."));
#if DEBUG
            qDebug() << "The connection was refused by the peer. Make sure the PdS server is running and check that the host name and port settings are correct.";
#endif
            break;
        default:
            QMessageBox::information(this, tr("PdS Client"),tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
#if DEBUG
            qDebug() << QString("The following error occurred: %1.").arg(tcpSocket->errorString());
#endif
    }
    this->close();
}

void Client::enableLogButton() {
    uiLog->LoginButton->setEnabled((!networkSession || networkSession->isOpen()) && !uiLog->UsernameEdit->text().isEmpty() && !uiLog->PasswordEdit->text().isEmpty());

}

void Client::sessionOpened() {
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

}

void Client::sendCredentials() {
     uiLog->LoginButton->setEnabled(false);
     uiLog->UsernameEdit->setReadOnly(true);
     uiLog->PasswordEdit->setReadOnly(true);
     uiLog->UsernameEdit->clearFocus();
     uiLog->RegistrationLink->setTextInteractionFlags(Qt::NoTextInteraction);
     uiLog->CancellationLink->setTextInteractionFlags(Qt::NoTextInteraction);
     uiLog->RegistrationLink->setCursor(QCursor(Qt::ArrowCursor));
     uiLog->CancellationLink->setCursor(QCursor(Qt::ArrowCursor));
     logStatusBar->showMessage(tr("Connected to the server."));
#if DEBUG
    qDebug() << "Connected to the server.";
#endif

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
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
            logStatusBar->showMessage(tr("Could not send message."), 3000);
        }
        this->loggedUsername = uiLog->UsernameEdit->text();
        tcpSocket->flush();
    }
	
    logStatusBar->showMessage(tr("Credential sent, waiting for reply..."), 3000);
#if DEBUG
    qDebug() << "Credential sent, waiting for reply...";
#endif
}

void Client::openRegistrationWindow() {
	this->setVisible(false);
	
	if (RegWin != nullptr) {
		RegWin.reset();
	}
	
	uiReg = std::make_shared<Ui::SignupWin> ();
	RegWin = std::make_shared<QDialog> ();
	uiReg->setupUi(RegWin.get());
	regStatusBar = std::make_shared<QStatusBar> (RegWin.get());
	uiReg->verticalLayout->addWidget(regStatusBar.get());
	uiReg->ProfilePicture->setPixmap(circularPixmap(QPixmap(defaultPicture)));
	uiReg->UsernameEdit->setText(uiLog->UsernameEdit->text());
	
	regHidePassword = uiReg->PasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	regPasswordButton = qobject_cast<QToolButton *>(regHidePassword->associatedWidgets().last());
	regPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(regPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiReg->PasswordEdit); });
	connect(regPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiReg->PasswordEdit); });
	
	regHideRepeatPassword = uiReg->RepeatPasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	regRepeatPasswordButton = qobject_cast<QToolButton *>(regHideRepeatPassword->associatedWidgets().last());
	regRepeatPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(regRepeatPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiReg->RepeatPasswordEdit); });
	connect(regRepeatPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiReg->RepeatPasswordEdit); });
	
	connect(uiReg->NameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->SurnameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RepeatPasswordEdit, &QLineEdit::textChanged, this, &Client::enableRegButton);
	connect(uiReg->RegisterButton, &QPushButton::released, this, &Client::requestRegistration);
	connect(uiReg->ProfilePictureButton, &QPushButton::released, this, [this](){ uploadProfilePicture(uiReg->ProfilePicture, uiReg->DeletePictureButton); });
	connect(uiReg->DeletePictureButton, &QPushButton::released, this, [this](){ deleteProfilePicture(uiReg->ProfilePicture, uiReg->DeletePictureButton); });
	connect(RegWin.get(), &QDialog::finished, this, &Client::reactivateLoginWindow);
	
	connect(uiReg->InfoUsername, &QPushButton::released, this, [this](){
		QMessageBox::information(this, tr("Username information"), tr("The username should be at least 4 and at most 16 characters long.\nIt should not contain space characters."));
	});
	
	connect(uiReg->InfoPassword, &QPushButton::released, this, [this](){
		QMessageBox::information(this, tr("Password information"), tr("The password should be at least 8 and at most 16 characters long.\nIt should not contain space characters.\nIt should contain at least three among lowercase letters, uppercase letters, special characters (._!?@) and numbers."));
	});
	
    RegWin->show();
}

void Client::uploadProfilePicture(QLabel* label, QPushButton *deleteButton) {
#if DEBUG
    qDebug() << "Uploading profile picture";
#endif

	QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.jpg *.png *.jpeg *.bmp *.JPG *.PNG *.JPEG *.BMP)"));
	if (QString::compare(filename, QString()) != 0) {
		QImage image;
		bool valid = image.load(filename);
		if (valid) {
			label->setPixmap(circularPixmap(QPixmap::fromImage(image)));
			deleteButton->setEnabled(true);
		}
	}
}

void Client::deleteProfilePicture(QLabel* label, QPushButton *deleteButton) {
#if DEBUG
    qDebug() << "Deleting profile picture";
#endif

	label->setPixmap(circularPixmap(QPixmap(defaultPicture)));
	deleteButton->setEnabled(false);
	deleteButton->parentWidget()->setFocus();
}

void Client::enableRegButton() {
	if (uiReg->UsernameEdit->text() == QString("___default_img")) {
		regStatusBar->showMessage(tr("This username is not valid."));
		uiReg->RegisterButton->setEnabled(false);
		return;
	}
	
	if (!uiReg->NameEdit->text().isEmpty() & !uiReg->SurnameEdit->text().isEmpty() & !uiReg->UsernameEdit->text().isEmpty() & !uiReg->PasswordEdit->text().isEmpty() & !uiReg->RepeatPasswordEdit->text().isEmpty()) {
		if (uiReg->PasswordEdit->text() != uiReg->RepeatPasswordEdit->text() | !checkPasswordFormat(uiReg->PasswordEdit->text().toStdString()) | !checkUsernameFormat(uiReg->UsernameEdit->text().toStdString())) {
			uiReg->RegisterButton->setEnabled(false);
		} else {
			uiReg->RegisterButton->setEnabled(true);
		}
	} else {
		uiReg->RegisterButton->setEnabled(false);
	}
	
	if (!checkUsernameFormat(uiReg->UsernameEdit->text().toStdString()) & !uiReg->UsernameEdit->text().isEmpty()) {
		if (regStatusBar->currentMessage() != QString(tr("The username format is not valid."))) {
			regStatusBar->showMessage(tr("The username format is not valid."));
		}
		return;
	} else {
		regStatusBar->clearMessage();
	}
	
	if (!checkPasswordFormat(uiReg->PasswordEdit->text().toStdString()) & !uiReg->PasswordEdit->text().isEmpty()) {
		if (regStatusBar->currentMessage() != QString(tr("The password format is not valid."))) {
			regStatusBar->showMessage(tr("The password format is not valid."));
		}
		return;
	} else {
		regStatusBar->clearMessage();
	}
	
	if (uiReg->PasswordEdit->text() != uiReg->RepeatPasswordEdit->text()) {
		if (regStatusBar->currentMessage() != QString(tr("The two passwords do not coincide!"))) {
			regStatusBar->showMessage(tr("The two passwords do not coincide!"));
		}
	} else {
		regStatusBar->clearMessage();
	}
}

void Client::requestRegistration() {
	uiReg->NameEdit->setReadOnly(true);
	uiReg->SurnameEdit->setReadOnly(true);
	uiReg->UsernameEdit->setReadOnly(true);
	uiReg->PasswordEdit->setReadOnly(true);
	uiReg->RepeatPasswordEdit->setReadOnly(true);
	
	regStatusBar->showMessage(tr("Checking database..."), 3000);
#if DEBUG
    qDebug() << "Checking database...";
#endif

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
    message["propic"]= jsonValFromPixmap(*uiReg->ProfilePicture->pixmap());

    // send the JSON using QDataStream
    out << QJsonDocument(message).toJson();

    if (!tcpSocket->write(block)) {
	    QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
	    regStatusBar->showMessage(tr("Could not send message."), 3000);
    }
    this->loggedUsername = uiReg->UsernameEdit->text();
    tcpSocket->flush();
}

void Client::reactivateLoginWindow() {
	uiReg.reset();
	regStatusBar.reset();
	uiCanc.reset();
	cancStatusBar.reset();
	
	uiLog->LoginButton->setEnabled(!(uiLog->UsernameEdit->text().isEmpty() | uiLog->PasswordEdit->text().isEmpty()));
	uiLog->RegistrationLink->clearFocus();
	uiLog->CancellationLink->clearFocus();
	QWidget::setTabOrder(uiLog->UsernameEdit, uiLog->PasswordEdit);
	QWidget::setTabOrder(uiLog->PasswordEdit, uiLog->LoginButton);
	uiLog->UsernameEdit->setFocus();
	this->setVisible(true);
}

void Client::openCancellationWindow() {
	this->setVisible(false);
	
	if (CancWin != nullptr) {
		CancWin.reset();
	}
	
	uiCanc = std::make_shared<Ui::DeleteAccountWin> ();
	CancWin = std::make_shared<QDialog> ();
	uiCanc->setupUi(CancWin.get());
	cancStatusBar = std::make_shared<QStatusBar> (CancWin.get());
	uiCanc->verticalLayout->addWidget(cancStatusBar.get());
	
	cancHidePassword = uiCanc->PasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	cancPasswordButton = qobject_cast<QToolButton *>(cancHidePassword->associatedWidgets().last());
	cancPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(cancPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiCanc->PasswordEdit); });
	connect(cancPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiCanc->PasswordEdit); });
	
	connect(uiCanc->UsernameEdit, &QLineEdit::textChanged, this, &Client::enableDelButton);
	connect(uiCanc->PasswordEdit, &QLineEdit::textChanged, this, &Client::enableDelButton);
	connect(uiCanc->DeleteButton, &QPushButton::released, this, &Client::requestDeletion);
	connect(CancWin.get(), &QDialog::finished, this, &Client::reactivateLoginWindow);
	
	CancWin->show();
}

void Client::enableDelButton() {
	uiCanc->DeleteButton->setEnabled(!uiCanc->UsernameEdit->text().isEmpty() & !uiCanc->PasswordEdit->text().isEmpty());
}

void Client::requestDeletion() {
	cancStatusBar->showMessage(tr("Deleting account..."), 3000);
#if DEBUG
    qDebug() << "Deleting account...";
#endif
	
	uiCanc->UsernameEdit->setReadOnly(true);
	uiCanc->PasswordEdit->setReadOnly(true);
	uiCanc->DeleteButton->setEnabled(false);
	
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
	        QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
	        cancStatusBar->showMessage(tr("Could not send message."), 3000);
        }
        tcpSocket->flush();
    }

    logStatusBar->showMessage(tr("Credential sent, waiting for reply..."), 3000);
#if DEBUG
    qDebug() << "Credential sent, waiting for reply...";
#endif
}

void Client::openWelcomeWin2(bool firstTime) {
    uiChoice = std::make_shared<Ui::WelcomeWinClone> ();
    ChoiceWin = std::make_shared<QDialog> ();
    uiChoice->setupUi(ChoiceWin.get());
    ChoiceWin->setWindowTitle("PiDiEsse - " + loggedUser->getUsername());
    uiChoice->OpenTable->setRowCount(avail_file.size());
    uiChoice->OpenTable->setColumnCount(1);
    auto cbModel = new QStringListModel;
    uiChoice->ProfilePicture->setPixmap(circularPixmap(const_cast<QPixmap &&>(loggedUser->getPropic()), 100, loggedUser->getColor()));

    if (firstTime) {
        uiChoice->WelcomeLabel->setText(tr("Welcome,\n%1!").arg(loggedUser->getName()));
    } else {
        uiChoice->WelcomeLabel->setText(tr("Welcome back,\n%1!").arg(loggedUser->getName()));
    }

    QStringList fileList;
    for(auto s:avail_file){
        fileList+=s;
    }

    int i = 0;
    for (auto &file: fileList) {
        QTableWidgetItem *item = new QTableWidgetItem(file);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        uiChoice->OpenTable->setItem(i, 0, item);
        i++;
    }

    connect(uiChoice->NewButton, &QPushButton::released, this, &Client::openNewFileWin);
    connect(uiChoice->OpenButton, &QPushButton::released, this, &Client::openExistingFile);
    connect(uiChoice->SettingsButton, &QPushButton::released, this, &Client::openSettingsWindow);
    connect(uiChoice->LogoutButton, &QPushButton::released, this, &Client::requestLogout);
    connect(uiChoice->RefreshButton, &QPushButton::released, this, &Client::refreshFileList);


    ChoiceWin->show();
}

void Client::openNewFileWin() {
	ChoiceWin->setVisible(false);

    uiNewFile = std::make_shared<Ui::NewFile> ();
    NewFileWin = std::make_shared<QDialog>();
    uiNewFile->setupUi(NewFileWin.get());

    uiNewFile->newFileButtton->setDefault(true);
    connect(uiNewFile->newFileButtton, &QPushButton::released, this, [this](){
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
            message["header"] = "newfile";
            message["filename"] = uiNewFile->filenameInput->text();
            message["username"] = loggedUser->getUsername();
            // send the JSON using QDataStream
            out << QJsonDocument(message).toJson();

            if (!tcpSocket->write(block)) {
                QMessageBox::information(this, tr("PiDiEsse"), tr("Could not send message.\nTry again later."));
                cancStatusBar->showMessage(tr("Could not send message."), 3000);
            }
            tcpSocket->flush();
        }
        uiNewFile.reset();
        NewFileWin->close();
        ChoiceWin->setVisible(true);
    });
    connect(uiNewFile->loadURIButton, &QPushButton::released, this, &Client::openLink);
    connect(uiNewFile->cancelButton, &QPushButton::released, this, [this](){
        uiNewFile.reset();
        NewFileWin->close();
        ChoiceWin->setVisible(true);
    });
    connect(NewFileWin.get(), &QDialog::rejected, this, [this](){
        uiNewFile.reset();
        NewFileWin->close();
        ChoiceWin->setVisible(true);
    });
    NewFileWin->show();
}

void Client::openExistingFile() {
    // control if file is selected or not
    if (uiChoice->OpenTable->selectedItems().isEmpty()) {
        QMessageBox::information(ChoiceWin.get(), tr("PdS Server"), tr("No file selected."), QMessageBox::Ok);
#if DEBUG
        qDebug() << "The file is not selected.";
#endif
        return;
    }

#if DEBUG
    qDebug() << "Opening selected file...";
#endif

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
        message["header"] = "openfile";
        message["username"] = this->loggedUsername;
        message["filename"] = uiChoice->OpenTable->selectedItems()[0]->text();
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
            cancStatusBar->showMessage(tr("Could not send message."), 3000);
        }
        tcpSocket->flush();
        // ChoiceWin->close();
        // NewFileWin->close();
    }
}

void Client::refreshFileList() {
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
	message["header"] = "refr";
	message["username"]=this->loggedUsername;
	
	// send the JSON using QDataStream
	out << QJsonDocument(message).toJson();
	
	if (!tcpSocket->write(block)) {
		QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
		regStatusBar->showMessage(tr("Could not send message."), 3000);
	}
	tcpSocket->flush();
}

void Client::requestLogout() {
#if DEBUG
    qDebug() << "Requesting the logout...";
#endif

	//close the socket. The server will automatically log out the user
	tcpSocket->disconnectFromHost();
    loggedUser.reset();
    avail_file.clear();


    //reopen the connection to allow another user to login
	ChoiceWin->close();
	logStatusBar->clearMessage();
	uiLog->UsernameEdit->clear();
	uiLog->PasswordEdit->clear();
	uiLog->UsernameEdit->setReadOnly(false);
	uiLog->PasswordEdit->setReadOnly(false);
	uiLog->PasswordEdit->clearFocus();
	uiLog->RegistrationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->CancellationLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	uiLog->RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
	uiLog->CancellationLink->setCursor(QCursor(Qt::PointingHandCursor));
	reactivateLoginWindow();
}

bool Client::eventFilter(QObject *object, QEvent *event) {
	if (event->type() == QEvent::FocusIn) {
		if (object == uiChoice->OpenTable) {
			uiChoice->NewButton->setDefault(false);
		}
	}
}

void Client::setFileList(QJsonObject& data){
	avail_file.clear();
    for(auto s: data["File list"].toArray()){
        avail_file.push_back(s.toString());
    }
}

QPixmap Client::pixmapFrom(const QJsonValue &val) {
    auto const encoded = val.toString().toLatin1();
    QPixmap p;
    p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
    return p;
}

QJsonValue Client::jsonValFromPixmap(const QPixmap &p) {
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    return {QLatin1String(encoded)};
}

void Client::openSettingsWindow() {
	// TODO: aggiungere un tasto e separare aggiornamento password da aggiornamento immagine di profilo
	ChoiceWin->setVisible(false);
	
	if (CancWin != nullptr) {
		CancWin.reset();
	}
	
	uiSett = std::make_shared<Ui::ProfileSettingsWin> ();
	SettWin = std::make_shared<QDialog> ();
	uiSett->setupUi(SettWin.get());
	settStatusBar = std::make_shared<QStatusBar> (SettWin.get());
	uiSett->verticalLayout->addWidget(settStatusBar.get());
	uiSett->ProfilePicture->setPixmap(circularPixmap(const_cast<QPixmap &&>(loggedUser->getPropic())));
	uiSett->NameEdit->setText(loggedUser->getName());
	uiSett->SurnameEdit->setText(loggedUser->getSurname());
	uiSett->UsernameEdit->setText(loggedUser->getUsername());
	
	settHideCurrentPassword = uiSett->CurrentPasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	settCurrentPasswordButton = qobject_cast<QToolButton *>(settHideCurrentPassword->associatedWidgets().last());
	settCurrentPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(settCurrentPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiSett->CurrentPasswordEdit); });
	connect(settCurrentPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiSett->CurrentPasswordEdit); });
	
	settHideNewPassword = uiSett->NewPasswordEdit->addAction(QIcon(":/misc/themes/material/not_see.png"), QLineEdit::TrailingPosition);
	settNewPasswordButton = qobject_cast<QToolButton *>(settHideNewPassword->associatedWidgets().last());
	settNewPasswordButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(settNewPasswordButton, &QToolButton::pressed, this, [this](){ pressPasswordButton(uiSett->NewPasswordEdit); });
	connect(settNewPasswordButton, &QToolButton::released, this, [this](){ releasePasswordButton(uiSett->NewPasswordEdit); });

	connect(uiSett->CurrentPasswordEdit, &QLineEdit::textChanged, this, [this](){
		if (!uiSett->CurrentPasswordEdit->text().isEmpty() & (uiSett->NewPasswordEdit->text().isEmpty() | checkPasswordFormat(uiSett->NewPasswordEdit->text().toStdString()))) {
			uiSett->UpdateButton->setEnabled(true);
		} else {
			uiSett->UpdateButton->setEnabled(false);
		}
	});
	
	connect(uiSett->NewPasswordEdit, &QLineEdit::textChanged, this, [this](){
		if (!uiSett->NewPasswordEdit->text().isEmpty() & !checkPasswordFormat(uiSett->NewPasswordEdit->text().toStdString())) {
			uiSett->UpdateButton->setEnabled(false);
			if (settStatusBar->currentMessage() != QString(tr("The password format is not valid."))) {
				settStatusBar->showMessage(tr("The password format is not valid."));
			}
		} else {
			if (!uiSett->CurrentPasswordEdit->text().isEmpty()) {
				uiSett->UpdateButton->setEnabled(true);
			}
			settStatusBar->clearMessage();
		}
	});
	
	connect(uiSett->UndoButton, &QPushButton::released, this, [this](){
		uiSett->ProfilePicture->setPixmap(circularPixmap(const_cast<QPixmap &&>(loggedUser->getPropic())));
		uiSett->UndoButton->setEnabled(false);
		uiSett->DeletePictureButton->setEnabled(true);
		SettWin->setFocus();
	});
	connect(uiSett->ProfilePictureButton, &QPushButton::released, this, [this](){
		uploadProfilePicture(uiSett->ProfilePicture, uiSett->DeletePictureButton);
		uiSett->UndoButton->setEnabled(true);
	});
	connect(uiSett->DeletePictureButton, &QPushButton::released, this, [this](){
		deleteProfilePicture(uiSett->ProfilePicture, uiSett->DeletePictureButton);
		uiSett->UndoButton->setEnabled(true);
	});
	connect(uiSett->UpdateButton, &QPushButton::released, this, &Client::requestUserUpdate);
	connect(SettWin.get(), &QDialog::finished, this, [this](){
		uiSett.reset();
		settStatusBar.reset();
		ChoiceWin->setVisible(true);
	});
	
	connect(uiSett->InfoPassword, &QPushButton::released, this, [this](){
		QMessageBox::information(this, tr("Password information"), tr("The password should be at least 8 and at most 16 characters long.\nIt should not contain space characters.\nIt should contain at least three among lowercase letters, uppercase letters, special characters (._!?@) and numbers."));
	});
	
	SettWin->show();
}

void Client::requestUserUpdate() {
	uiSett->CurrentPasswordEdit->setReadOnly(true);
	uiSett->NewPasswordEdit->setReadOnly(true);
	uiSett->DeletePictureButton->setEnabled(false);
	uiSett->ProfilePictureButton->setEnabled(false);
	uiSett->UndoButton->setEnabled(false);
	
	settStatusBar->showMessage(tr("Checking password..."), 3000);
#if DEBUG
    qDebug() << "Checking password...";
#endif
	
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
	message["header"] = "upd";
	message["username"] = uiSett->UsernameEdit->text();
	message["password"] = uiSett->CurrentPasswordEdit->text();
	message["newpassword"] = uiSett->NewPasswordEdit->text();
	message["propic"]= jsonValFromPixmap(*uiSett->ProfilePicture->pixmap());
	
	// send the JSON using QDataStream
	out << QJsonDocument(message).toJson();
	
	if (!tcpSocket->write(block)) {
		QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
		settStatusBar->showMessage(tr("Could not send message."), 3000);
	}
	tcpSocket->flush();
}

void Client::releasePasswordButton(QLineEdit *lineEdit) {
	QToolButton *button = qobject_cast<QToolButton *>(sender());
	button->setIcon(QIcon(":/misc/themes/material/not_see.png"));
	lineEdit->setEchoMode(QLineEdit::Password);
}

void Client::pressPasswordButton(QLineEdit *lineEdit) {
	QToolButton *button = qobject_cast<QToolButton *>(sender());
	button->setIcon(QIcon(":/misc/themes/material/see.png"));
	lineEdit->setEchoMode(QLineEdit::Normal);
}

bool Client::checkPasswordFormat(std::string password) {
    bool uppercase;
    bool lowercase;
    bool special;
    bool number;
    bool space;

    int correct = 0;

    std::regex uppercase_regex {"[A-Z]+"};
    std::regex lowercase_regex {"[a-z]+"};
    std::regex special_regex {"[._!?@]+"};
    std::regex number_regex {"[0-9]+"};
    std::regex space_regex {"[ ]+"};

    if(password.length() < 8 || password.length() > 16) {
        return false;
    } else {
        uppercase = std::regex_search(password, uppercase_regex);
        lowercase = std::regex_search(password, lowercase_regex);
        special = std::regex_search(password, special_regex);
        number = std::regex_search(password, number_regex);
        space = std::regex_search(password, space_regex);
	    
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

bool Client::checkUsernameFormat(std::string username) {
	bool space;
	std::regex space_regex {"[ ]+"};
	if(username.length() < 4 || username.length() > 16) {
		return false;
	} else {
		space = std::regex_search(username, space_regex);
		if (space) return false;
	}
	return true;
}

QPixmap Client::circularPixmap(QPixmap &&source, int size, const QColor& color) {
    QPixmap scaled = source.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QRect croppedSquare((scaled.size().width() - size)/2, (scaled.size().height() - size)/2, size, size);
    scaled = scaled.copy(croppedSquare);
    float boarderPix = float(size)/100 * 5;

    QImage img = scaled.toImage();
    img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QImage imageOut(img.size(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageOut);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(img);
    painter.setBrush(brush);
    painter.drawRoundedRect(1, 1, int(float(size) - boarderPix/2), int(float(size) - boarderPix/2), size, size);

    //jQPen pen(color);
    //jpen.setWidth(int(boarderPix));
    //jpen.setStyle(Qt::SolidLine);
    //jpainter.setPen(pen);
   //  painter.drawRoundedRect(int(boarderPix/2), int(boarderPix/2), int(float(size) - boarderPix), int(float(size) - boarderPix), size, size);
   //  painter.drawRoundedRect(int(boarderPix/2), int(boarderPix/2), int(float(size) - boarderPix), int(float(size) - boarderPix), size, size);

    return QPixmap::fromImage(img);
}

void Client::openLink() {
    if (uiNewFile->filenameInput->text().isEmpty()) {
        QMessageBox::information(ChoiceWin.get(), tr("PdS Server"), tr("Link not inserted, insert link and try again"), QMessageBox::Ok);
        qDebug() << "Link not inserted";
        return;
    }

    QString link = uiNewFile->filenameInput->text();
    //check if the link is in a valid format
    QString prefix=link.split("/").at(0);
    if(prefix!="PDS-SharedEditor"){
        QMessageBox::information(ChoiceWin.get(), tr("PdS Server"), tr("Invalid link format, check your insertion!"), QMessageBox::Ok);
        qDebug() << "Link not inserted";
        return;
    }

#if DEBUG
    qDebug() << "Opening inserted link...";
#endif

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
        message["header"] = "shareFile";
        message["username"] = this->loggedUsername;
        message["link"] = link;
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

#if DEBUG
        qDebug() << "Trying to open file at link " << link;
#endif

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
            cancStatusBar->showMessage(tr("Could not send message."), 3000);
        }
        tcpSocket->flush();
    }

    uiNewFile.reset();
    NewFileWin->close();
    ChoiceWin->setVisible(true);
}

