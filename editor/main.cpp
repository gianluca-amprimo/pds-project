#include <QApplication>
#include <QtCore/QResource>
#include "MainEditor.h"
#include "client.h"
#include <iostream>
#include "FracPosition.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QResource::registerResource("./icons.qrc");
    const std::wstring editorIdentifer = L"AAAA";

    Client client;
    client.show();
    return app.exec();

    // testing fracPosition

/*
    FracPosition fp1, fp2, result;
    fp1 = QString("0,62");
    fp2 = QString("0,5");
    result = fp1 + fp2;
    result = result.divideByTwo();

    qDebug() << fp1.getStringPosition() << " + " << fp2.getStringPosition() << " = " << result.getStringPosition();
    qDebug() << fp1.getStringPosition() << " > " << fp2.getStringPosition() << " = " << (fp1 > fp2);
    qDebug() << fp1.getStringPosition() << " < " << fp2.getStringPosition() << " = " << (fp1 < fp2);
    qDebug() << fp1.getStringPosition() << " <= " << fp2.getStringPosition() << " = " << (fp1 <= fp2);
    qDebug() << fp1.getStringPosition() << " >= " << fp2.getStringPosition() << " = " << (fp1 >= fp2);
*/



    /*
    MainEditor mainEditor(nullptr, editorIdentifer);
    mainEditor.show();
    return app.exec();
    */
}