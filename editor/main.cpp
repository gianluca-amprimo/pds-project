#include <QApplication>
#include <QtCore/QResource>
#include "MainEditor.h"
#include <iostream>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QResource::registerResource("./icons.qrc");
    QApplication::setApplicationDisplayName(MainEditor::tr("PiDiEsse"));
    const std::wstring editorIdentifer = L"AAAA";
    MainEditor mainEditor(nullptr, editorIdentifer);
    mainEditor.show();
    return app.exec();
}