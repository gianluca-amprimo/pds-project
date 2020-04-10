#ifndef CLIENT_H
#define CLIENT_H

#define LOCALHOST 1

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QStatusBar>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
namespace Ui {
	class LoginWindow;
	class RegistrationWindow;
	class CancellationWindow;
	class FileChoiceWindow;
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
    void pressPasswordButton();
    void releasePasswordButton();

    void openRegistrationWindow();
	void uploadProfilePicture();
	void deleteProfilePicture();
    void enableRegButton();
    void requestRegistration();
    void reactivateLoginWindow();

    void openCancellationWindow();
    void enableDelButton();
    void requestDeletion();
    
    void openFileChoiceWindow(QString username);
    void openNewFile();
    void openExistingFile();
    bool eventFilter(QObject *object, QEvent *event);

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString loginReply;
    QNetworkSession *networkSession = nullptr;
	void requestConnection();
	void setFileList(QJsonObject&);
	
	Ui::LoginWindow *uiLog;
	std::list<QString> avail_file;
	QStatusBar *logStatusBar;
	QToolButton *logPasswordButton;
	QAction *hidePassword;
	
	QDialog *RegWin;
	Ui::RegistrationWindow *uiReg;
	QStatusBar *regStatusBar;
	
	QDialog *CancWin;
	Ui::CancellationWindow *uiCanc;
	QStatusBar *cancStatusBar;
	
	QDialog *ChoiceWin;
	Ui::FileChoiceWindow *uiChoice;
};

#endif