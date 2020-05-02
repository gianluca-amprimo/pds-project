
#include <QApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr("PiDiEsse [server]"));
    Server server;
    server.show();
    return app.exec();
}
