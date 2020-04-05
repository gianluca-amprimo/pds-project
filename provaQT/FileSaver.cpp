//
// Created by antonio on 27/03/20.
//

#include "FileSaver.h"
#include <iostream>
#include <QtWidgets/QPlainTextEdit>
#include <fstream>


void FileSaver::save() {
    if(this->plainTextEdit != nullptr){
        std::wstring text= this->plainTextEdit->toPlainText().toStdWString();
        std::wofstream output;
        output.open(this->fileName);
        output << text << std::endl;

    }
}

FileSaver::FileSaver(QPlainTextEdit *plainTextEdit) {
    this->plainTextEdit = plainTextEdit;

}

void FileSaver::setFileName(std::string filename) {
    this->fileName = filename;

}
