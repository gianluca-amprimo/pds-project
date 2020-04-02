
#include "ui_provaText.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    Ui::MainWindow textProva{};
//
    textProva.setupUi(&mainWindow);

    mainWindow.show();
    return app.exec();

}