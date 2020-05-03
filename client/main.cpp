#include <QApplication>
#include <QtCore/QResource>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QResource::registerResource("./icons.qrc");
    QApplication::setApplicationDisplayName(Client::tr("PiDiEsse [client]"));
    Client client;
    client.show();
    return app.exec();
}
