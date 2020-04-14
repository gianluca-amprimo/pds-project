#ifndef CLIENT_H
#define CLIENT_H

#define LOCALHOST 1

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QStatusBar>
#include <memory>
#include "User.h"
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
namespace Ui {
	class LoginWindow;
	class RegistrationWindow;
	class CancellationWindow;
	class FileChoiceWindow;
	class SettingsWindow;
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
    
    void openFileChoiceWindow(bool firstTime);
    void openNewFile();
    void openExistingFile();
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

	Ui::LoginWindow *uiLog;
	std::list<QString> avail_file;
	QStatusBar *logStatusBar;
	QToolButton *logPasswordButton;
	QAction *logHidePassword;

	std::shared_ptr<User> loggedUser;
	QDialog *RegWin;
	Ui::RegistrationWindow *uiReg;
	QStatusBar *regStatusBar;
	QToolButton *regPasswordButton;
	QAction *regHidePassword;
	QToolButton *regRepeatPasswordButton;
	QAction *regHideRepeatPassword;
	
	QDialog *CancWin;
	Ui::CancellationWindow *uiCanc;
	QStatusBar *cancStatusBar;
	QToolButton *cancPasswordButton;
	QAction *cancHidePassword;
	
	QDialog *ChoiceWin;
	Ui::FileChoiceWindow *uiChoice;
	
	QDialog *SettWin;
	Ui::SettingsWindow *uiSett;
	QStatusBar *settStatusBar;
	QToolButton *settCurrentPasswordButton;
	QAction *settHideCurrentPassword;
	QToolButton *settNewPasswordButton;
	QAction *settHideNewPassword;
};

#endif