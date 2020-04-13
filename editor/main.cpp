#include <QApplication>
#include "MainEditor.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(MainEditor::tr("PiDiEsse"));
    MainEditor mainEditor;
    mainEditor.show();
    return app.exec();
}