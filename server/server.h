#ifndef SERVER_H
#define SERVER_H

#define LOCALHOST 1

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QColor>
#include <map>
#include <list>
#include <memory>
#include "exceptions.h"
#include "Session.h"
#include "FracPosition.h"

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
    void handleDisconnect();

    bool checkUser(QJsonObject &, QTcpSocket *);
    bool registerUser(QJsonObject &, QTcpSocket *);
    bool cancelUser(QJsonObject &, QTcpSocket *);
    bool updateUser(QJsonObject &, QTcpSocket *);
    void processUserRequest();

    void getConnectedSocket();

	bool refreshFileList(QJsonObject &, QTcpSocket *);
	bool createFile(QJsonObject &data, QTcpSocket *active_socket);
	bool openFile(QJsonObject &data, QTcpSocket *active_socket);
    bool closeFileReq(QJsonObject &data, QTcpSocket *active_socket);
    bool saveFileReq(QJsonObject &data, QTcpSocket *active_socket);
    bool receiveChar(QJsonObject &data, QTcpSocket *active_socket);
    bool receiveBatchChar(QJsonArray &data, QTcpSocket *active_socket);
    bool deleteChar(QJsonObject &data, QTcpSocket *active_socket);
    bool deleteBatchChar(QJsonObject &data, QTcpSocket *active_socket);


    void sendColors(QString filename);
    bool updatePosition(QJsonObject &jSobject, QTcpSocket *active_socket);

private:
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    QDataStream in;
    QNetworkSession *networkSession = nullptr;

    // variables for handling the users
    QHash<QString, QTcpSocket*> idleConnectedUsers;      // utente -> socket
    QHash<QString, std::shared_ptr<Session>> active_sessions;            // filename -> sessione
    QHash<QString, QTimer*> timers;                      // filename -> timers

    QJsonObject prepareJsonReply(QString header, QString result, QString username, bool propic=false, bool filelist=false, bool personal_info=false);
    QJsonValue jsonValFromPixmap(const QPixmap &p);
    QPixmap pixmapFrom(const QJsonValue &val);

    void printConsole(QString msg, bool err = false);   // function to print on the console of the server
    bool checkPasswordFormat(QString password);
    void sendMessage(QJsonObject message, QTcpSocket*);
    void sendMessage(QJsonArray message, QTcpSocket*);

    QColor generateColor(); // function to generate a random color for a user
    bool removeUserFromSession(std::shared_ptr<Session> session, const QString& username, std::optional<std::shared_ptr<QJsonObject>> returnMessage);

    bool
    saveFile(const QString &filename, const QByteArray &content,
             std::optional<std::shared_ptr<QJsonObject>> returnMessage);
};

#endif
