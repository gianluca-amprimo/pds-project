#ifndef SERVER_H
#define SERVER_H

#define LOCALHOST 1

#include <QDialog>
#include <QString>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QDialog {
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    ~Server() override;

private slots:
    void sessionOpened();
    void checkUser();
    void getConnectedSocket();
private:
    void printConsole(std::string &&msg, bool err = false);
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *active_socket = nullptr;
    QDataStream in;
    QNetworkSession *networkSession = nullptr;
};

#endif
