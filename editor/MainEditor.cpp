#include "MainEditor.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <iomanip>


MainEditor::MainEditor(QWidget *parent, std::string editorIdentifier) :
    QMainWindow(parent),
    ui(new Ui::MainEditor) {
    ui->setupUi(this);
    initUI();
    saveAsDialog = new SaveAsDialog(this, ui->textArea);

    this->thisEditorIdentifier = editorIdentifier;
    this->ui->textArea->setThisEditorIdentifier(editorIdentifier);

    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});

}

void MainEditor::initUI() {
    auto separator = ui->toolBar->insertSeparator(ui->bold);
    this->fontSelector = new QFontComboBox;
    ui->toolBar->insertWidget(separator, fontSelector);
}

MainEditor::~MainEditor() {
    delete ui;
}

