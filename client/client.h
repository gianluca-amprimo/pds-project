#ifndef CLIENT_H
#define CLIENT_H

#define LOCALHOST 1

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
namespace Ui {
	class LoginWindow;
	class RegistrationWindow;
	class CancellationWindow;
}
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
	void requestLogin();
    void readResponse();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableLogButton();
    void sessionOpened();
    void sendCredentials();

    void openRegistrationWindow();
    void enableRegButton();
    void requestRegistration();
    void reactivateLoginWindow();

    void openCancellationWindow();
    void enableDelButton();
    void requestDeletion();

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString loginReply;
    QNetworkSession *networkSession = nullptr;
	
	Ui::LoginWindow *uiLog;
	QStatusBar *logStatusBar;
	
	QDialog *RegWin;
	Ui::RegistrationWindow *uiReg;
	QStatusBar *regStatusBar;
	
	QDialog *CancWin;
	Ui::CancellationWindow *uiCanc;
	QStatusBar *cancStatusBar;
};

#endif