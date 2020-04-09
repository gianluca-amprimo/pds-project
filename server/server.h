#ifndef SERVER_H
#define SERVER_H

#define LOCALHOST 1

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <map>
#include <list>
#include "User.h"

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
    void processUserRequest();
    void handleDisconnect();
    bool checkUser(std::string, QTcpSocket*);
    bool registerUser(std::string, QTcpSocket*);
    bool cancelUser(std::string, QTcpSocket*);
    void getConnectedSocket();
private:
    void printConsole(std::string &&msg, bool err = false);
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    std::map<int, QTcpSocket*>  active_sockets; //stores all the connected sockets with different clients
    QDataStream in;
    QNetworkSession *networkSession = nullptr;
    std::map<User, std::list<QTcpSocket*>> activeUsers; //map che contiene key-pair: utente-socket a lui associati (se runna due volte il cliente sono più di uno)
    std::map<QString,std::list<User>> openDocuments; //lista di file aperti e chi ci sta lavorando

};

#endif
