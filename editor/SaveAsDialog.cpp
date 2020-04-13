#include "SaveAsDialog.h"
#include <iostream>
#include <fstream>

SaveAsDialog::SaveAsDialog(QWidget *parent, QTextEdit *textArea) :
    QDialog(parent),
    ui(new Ui::SaveAsDialog) {
    ui->setupUi(this);
    this->textArea = textArea;
}

void SaveAsDialog::save() {
    if(this->textArea != nullptr){
        std::wstring text= this->textArea->toPlainText().toStdWString();
        std::wofstream output;
        output.open(this->fileName);
        output << text << std::endl;
    }
}

void SaveAsDialog::setFileName(std::string &&filename) {
    this->fileName = filename;
}

SaveAsDialog::~SaveAsDialog() {
    delete ui;
}


