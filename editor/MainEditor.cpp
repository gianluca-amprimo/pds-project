#include "MainEditor.h"
#include <QtWidgets/QDialogButtonBox>

MainEditor::MainEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainEditor) {
    ui->setupUi(this);
    initIcons();
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

void MainEditor::initIcons() {
    ui->bold->setIcon( QIcon(":/text_styling/themes/material/bold.png") );
    ui->italic->setIcon( QIcon(":/text_styling/themes/material/italic.png") );
    ui->underline->setIcon( QIcon(":/text_styling/themes/material/underline.png") );
    ui->copy->setIcon(QIcon(":/text_edit/themes/material/copy.png") );
    ui->cut->setIcon(QIcon(":/text_edit/themes/material/cut.png") );
    ui->paste->setIcon(QIcon(":/text_edit/themes/material/paste.png") );
    ui->open->setIcon(QIcon(":/file/themes/material/new_doc.png"));
    ui->saveAs->setIcon(QIcon(":/file/themes/material/save_as.png"));
    ui->save->setIcon(QIcon(":/file/themes/material/save.png"));
}


