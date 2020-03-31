#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
namespace Ui { class Client; }
QT_END_NAMESPACE

//! [0]
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

private:

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString loginReply;
    Ui::Client *ui;
    QNetworkSession *networkSession = nullptr;
};
//! [0]

#endif