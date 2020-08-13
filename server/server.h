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
#include "Session.h"

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
	bool refreshFileList(QJsonObject &, QTcpSocket *);
	bool createFile(QJsonObject &data, QTcpSocket *active_socket);
	bool openFile(QJsonObject &data, QTcpSocket *active_socket);
    bool saveFile(QJsonObject &data, QTcpSocket *active_socket);
    bool receiveSymbol(QJsonObject &data, QTcpSocket *active_socket);

private:
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    // QList<QTcpSocket*>  active_sockets; //stores all the connected sockets with different clients, mi serve? Va aggiustata in caso
    QDataStream in;
    QNetworkSession *networkSession = nullptr;

    // variables for handling the users
    QMap<User, QTcpSocket*> idleConnectedUsers; // map utente-socket a lui associati (se runna due volte il cliente sono più di uno)
    //std::map<QString, QList<User>> openFiles; // lista di file aperti e chi ci sta lavorando
    QMap<User, QString> userColorMap; // map to associate a color to a user

    QMap<QString, Session*> active_sessions; // questa dovrebbe contenere tutte le associazioni necessarie

    void printConsole(QString msg, bool err = false); // function to print on the console of the server
    QJsonObject prepareJsonReply(QString header, QString result, QString username, bool propic=false, bool filelist=false, bool personal_info=false);
    QJsonValue jsonValFromPixmap(const QPixmap &p);
    QPixmap pixmapFrom(const QJsonValue &val);
    bool checkPasswordFormat(QString password);
    void sendMessage(QJsonObject message, QTcpSocket*);
};

#endif
