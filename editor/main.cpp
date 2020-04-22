#include <QApplication>
#include <QtCore/QResource>
#include "MainEditor.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QResource::registerResource("./icons.qrc");
    QApplication::setApplicationDisplayName(MainEditor::tr("PiDiEsse"));
    MainEditor mainEditor;
    mainEditor.show();
    return app.exec();
}