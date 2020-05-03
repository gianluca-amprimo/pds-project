#ifndef CLIENT_H
#define CLIENT_H

#define LOCALHOST 1

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QStatusBar>
#include <memory>
#include "User.h"
#include "MainEditor.h"
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
namespace Ui {
	class LoginWin;
	class SignupWin;
	class DeleteAccountWin;
	class WelcomeWin;
	class ProfileSettingsWin;
}
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void readResponse();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableLogButton();
    void sessionOpened();
    void sendCredentials();
	
    void openRegistrationWindow();
	void uploadProfilePicture(QLabel *label, QPushButton *deleteButton);
	void deleteProfilePicture(QLabel* label, QPushButton *deleteButton);
    void enableRegButton();
    void requestRegistration();
    void reactivateLoginWindow();

    void openCancellationWindow();
    void enableDelButton();
    void requestDeletion();
    
    void openWelcomeWin(bool firstTime);
    void openNewFile();
    void openExistingFile();
    void refreshFileList();
    void requestLogout();
    bool eventFilter(QObject *object, QEvent *event);
    
    void openSettingsWindow();
    void requestUserUpdate();
    
    void pressPasswordButton(QLineEdit *lineEdit);
	void releasePasswordButton(QLineEdit *lineEdit);

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString loginReply;
    QNetworkSession *networkSession = nullptr;
	void requestConnection();
    QPixmap pixmapFrom(const QJsonValue &val);
    QJsonValue jsonValFromPixmap(const QPixmap &p);
	void setFileList(QJsonObject&);
	bool checkPasswordFormat(std::string password);
	bool checkUsernameFormat(std::string username);
	
	std::shared_ptr<Ui::LoginWin> uiLog;
	std::list<QString> avail_file;
	std::shared_ptr<QStatusBar> logStatusBar;
	QToolButton *logPasswordButton;
	QAction *logHidePassword;


	std::shared_ptr<User> loggedUser;
	
	std::shared_ptr<QDialog> RegWin;
	std::shared_ptr<Ui::SignupWin> uiReg;
	std::shared_ptr<QStatusBar> regStatusBar;
	QToolButton *regPasswordButton;
	QAction *regHidePassword;
	QToolButton *regRepeatPasswordButton;
	QAction *regHideRepeatPassword;
	
	std::shared_ptr<QDialog> CancWin;
	std::shared_ptr<Ui::DeleteAccountWin> uiCanc;
	std::shared_ptr<QStatusBar> cancStatusBar;
	QToolButton *cancPasswordButton;
	QAction *cancHidePassword;
	
	std::shared_ptr<QDialog> ChoiceWin;
	std::shared_ptr<Ui::WelcomeWin> uiChoice;
	
	std::shared_ptr<QDialog> SettWin;
	std::shared_ptr<Ui::ProfileSettingsWin> uiSett;
	std::shared_ptr<QStatusBar> settStatusBar;
	QToolButton *settCurrentPasswordButton;
	QAction *settHideCurrentPassword;
	QToolButton *settNewPasswordButton;
	QAction *settHideNewPassword;

	std::shared_ptr<MainEditor> MainEditorWin;
};

#endif