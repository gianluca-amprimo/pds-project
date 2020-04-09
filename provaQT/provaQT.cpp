
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

    textProva.setupUi(&mainWindow);
    saveDialog.setupUi(saveAsDialog);
    FileSaver* fs = new FileSaver(textProva.textEdit);

    QObject::connect(textProva.pushButton, SIGNAL(pressed()), fs, SLOT(save()));
    QObject::connect(textProva.actionSave_As, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveDialog.buttonBox, &QDialogButtonBox::accepted, fs, [=](){fs->setFileName(saveDialog.lineEdit->text().toStdString());});

    textProva.textEdit->setCursorWidth(4);
    QTextCursor cursor2 = QTextCursor(textProva.textEdit->textCursor());
    cursor2.setPosition(4);
    cursor2.keepPositionOnInsert();
    cursor2.insertText("Ciao io mi chiamo antonio");

    QTextCursor cursor3 = cursor2;
    cursor3.setPosition(0);

    mainWindow.show();
    return app.exec();

}