//
// Created by antonio on 27/03/20.
//

#include "FileSaver.h"
#include <iostream>
#include <QtWidgets/QPlainTextEdit>
#include <fstream>


void FileSaver::save() {
    if(this->textEdit != nullptr){
        std::wstring text= this->textEdit->toPlainText().toStdWString();
        std::wofstream output;
        output.open(this->fileName);
        output << text << std::endl;

    }
}

FileSaver::FileSaver(QTextEdit *textEdit) {
    this->textEdit = textEdit;

}

void FileSaver::setFileName(std::string filename) {
    this->fileName = filename;

}
