#ifndef SERVER_H
#define SERVER_H

#define LOCALHOST 1

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QColor>

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
    bool checkUser(QJsonObject &, QTcpSocket *);
    bool registerUser(QJsonObject &, QTcpSocket *);
    bool cancelUser(QJsonObject &, QTcpSocket *);
    bool updateUser(QJsonObject &, QTcpSocket *);
    void getConnectedSocket();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    // std::list<QTcpSocket*>  active_sockets; //stores all the connected sockets with different clients, mi serve? Va aggiustata in caso
    QDataStream in;
    QNetworkSession *networkSession = nullptr;

    // variables for handling the users
    std::map<User, std::list<QTcpSocket *>> activeUsers; // map utente-socket a lui associati (se runna due volte il cliente sono più di uno)
    std::map<QString, std::list<User>> openFiles; // lista di file aperti e chi ci sta lavorando
    std::map<User, std::string> userColorMap; // map to associate a color to a user

    void printConsole(std::string &&msg, bool err = false); // function to print on the console of the server
    QJsonObject prepareJsonWithFileList(QString, QString, std::string);
    QJsonValue jsonValFromPixmap(const QPixmap &p);
    QPixmap pixmapFrom(const QJsonValue &val);
};

#endif
