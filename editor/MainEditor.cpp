#include "MainEditor.h"
#include <QtWidgets/QDialogButtonBox>

MainEditor::MainEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainEditor) {
    ui->setupUi(this);
    saveAsDialog = new SaveAsDialog(this, ui->textArea);

    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});

    ui->textArea->setCursorWidth(2);
    QTextCursor cursor2 = QTextCursor(ui->textArea->textCursor());
    cursor2.setPosition(0);
    cursor2.keepPositionOnInsert();
    cursor2.insertText("Ciao io mi chiamo antonio");

    QTextCursor cursor3 = cursor2;
    cursor3.setPosition(0);
}

MainEditor::~MainEditor() {
    delete ui;
}


