#include <QApplication>
#include <QtCore/QResource>
#include "MainEditor.h"
#include "client.h"
#include <iostream>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QResource::registerResource("./icons.qrc");
    const std::wstring editorIdentifer = L"AAAA";

    Client client;
    client.show();
    return app.exec();

    /*
    MainEditor mainEditor(nullptr, editorIdentifer);
    mainEditor.show();
    return app.exec();
    */
}