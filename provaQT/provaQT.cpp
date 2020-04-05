
#include "ui_provaText.h"
#include "ui_saveDialog.h"
#include <QObject>
#include <QApplication>
#include "FileSaver.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QMainWindow mainWindow;
    auto* saveAsDialog = new QDialog();
    Ui::MainWindow textProva{};
    Ui::Dialog saveDialog{};
//

    textProva.setupUi(&mainWindow);
    saveDialog.setupUi(saveAsDialog);
    FileSaver* fs = new FileSaver(textProva.plainTextEdit);
    QObject::connect(textProva.pushButton, SIGNAL(pressed()), fs, SLOT(save()));
    QObject::connect(textProva.actionSave_As, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveDialog.buttonBox, &QDialogButtonBox::accepted, fs, [=](){fs->setFileName(saveDialog.lineEdit->text().toStdString());});



    mainWindow.show();
    return app.exec();

}