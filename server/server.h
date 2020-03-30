#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>

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

private:
    Ui::Server *ui;
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;
};

#endif
