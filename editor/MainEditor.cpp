#include "MainEditor.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <iomanip>


MainEditor::MainEditor(QWidget *parent, std::string editorIdentifier) :
    QMainWindow(parent),
    ui(new Ui::MainEditor) {
    ui->setupUi(this);
    initIcons();
    saveAsDialog = new SaveAsDialog(this, ui->textArea);

    this->thisEditorIdentifier = editorIdentifier;
    this->charCounter = 0;
    this->currentPosition = 0;
    this->oldPosition = 0;

    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});
    QObject::connect(ui->textArea, SIGNAL(textChanged()), this, SLOT(notifyChange()));
    QObject::connect(ui->textArea, SIGNAL(cursorPositionChanged()), this, SLOT(notifyMove()));

}

void MainEditor::initIcons() {
    ui->bold->setIcon( QIcon(":/text_styling/themes/material/bold.png") );
    ui->italic->setIcon( QIcon(":/text_styling/themes/material/italic.png") );
    ui->underline->setIcon( QIcon(":/text_styling/themes/material/underline.png") );
    ui->copy->setIcon(QIcon(":/text_edit/themes/material/copy.png") );
    ui->cut->setIcon(QIcon(":/text_edit/themes/material/cut.png") );
    ui->paste->setIcon(QIcon(":/text_edit/themes/material/paste.png") );
    ui->newDoc->setIcon(QIcon(":/file/themes/material/new_doc.png"));
    ui->openDoc->setIcon(QIcon(":/file/themes/material/open_doc.png"));
    ui->save->setIcon(QIcon(":/file/themes/material/save.png"));
    ui->saveAs->setIcon(QIcon(":/file/themes/material/save_as.png"));
    ui->activeUsers->setIcon(QIcon(":/file/themes/material/user.png"));
    ui->alignCenter->setIcon(QIcon(":/text_align/themes/material/align_center.png"));
    ui->alignLeft->setIcon(QIcon(":/text_align/themes/material/align_left.png"));
    ui->alignRight->setIcon(QIcon(":/text_align/themes/material/align_right.png"));
    ui->alignJustified->setIcon(QIcon(":/text_align/themes/material/align_justified.png"));
    auto separator = ui->toolBar->insertSeparator(ui->bold);
    this->fontSelector = new QFontComboBox;
    ui->toolBar->insertWidget(separator, fontSelector);
}

MainEditor::~MainEditor() {
    delete ui;
}

void MainEditor::notifyChange() {
    char changed;
    int insertType;

    // this is all the algorithm to compute the fractionary position;
    if(this->oldPosition > this->currentPosition){

        std::cout << "Character deleted" << std::endl;
    }else{
        // Prendo le informazioni necessarie per costruire il simbolo
        this->charCounter++;
        changed = this->ui->textArea->toPlainText().toStdString()[this->currentPosition-1];

        // calcolo l'id rispetto al mio editor
        int localCharIdLen = std::to_string(this->charCounter).length();
        std::vector<int> position;
        std::string localCharId  = std::string(6-localCharIdLen, '0') + std::to_string(charCounter);
        std::string charId = this->thisEditorIdentifier + localCharId;
        std::cout << "char has id " << charId << std::endl;
        int last;

        // calcolo la posizione frazionaria
        // se il carattere è inserito in fondo basta solo vedere il primo intero della posizione frazionaria
        // se invece è in mezzo bisogna trovare l'intero

        // last
        if(this->currentPosition == this->_symbols.size()+1){ // +1 because char is already inserted
            insertType = BACK;
            std::cout << "Inserting at the back" << std::endl;
            // take just the first number of the last element and add 1
            std::vector<int> fracPosition;

            //position of last char
            // first letter inserted
            if (!this->_symbols.size()) {
                last = 0;
                fracPosition.push_back(last);
                fracPosition.push_back(5);
            } else {
                // last letter inserted
                last = this->_symbols.back().getPosition().front() + 1;
                fracPosition.push_back(last);
            }
            position = fracPosition;
        }else if(this->currentPosition == 1){
            insertType = HEAD;
           // retrieve prior first element
           std::cout << "Inserting at the head" << std::endl;
           std::vector<int> oldFirstPos = this->_symbols.front().getPosition();
           int numDecimals = oldFirstPos.size();
           for(int i = 0; i < numDecimals; i++)
               position.push_back(0);

           position.push_back(5);

        }else if(this->currentPosition > 1 && this->currentPosition < this->_symbols.size()+1) {
            insertType = MIDDLE;
            std::cout << "Inserting in the middle" << std::endl;
            std::vector<int> prevPos = this->_symbols.at(this->currentPosition - 2).getPosition();
            std::vector<int> nextPos = this->_symbols.at(this->currentPosition-1).getPosition();

            // this should be enough to consider both the comprehending symbols

            position = prevPos;
            position.push_back(5);
            std::string strPosition = "";
            for(int i = 0; i < prevPos.size(); i++){
                strPosition += prevPos.at(i) + '0';
            }
            std::cout << "Prev pos is: " << strPosition << std::endl;
            strPosition = "";
            for(int i = 0; i < nextPos.size(); i++){
                strPosition += nextPos.at(i) + '0';
            }
            std::cout << "Next pos is: " << strPosition << std::endl;
            if (position == nextPos) {
                position.pop_back();
                position.push_back(0);
                position.push_back(5);
            }
        }

        // get string of fractionary position

        std::string strPosition = "";
        for(int i = 0; i < position.size(); i++){
           strPosition += position.at(i) + '0';
        }

        std::cout << "fractionary position " << strPosition  << std::endl;

        Symbol symbol(changed, charId, position);
        switch(insertType){
            case BACK:
                this->_symbols.push_back(symbol);
                break;
            case HEAD:
                this->_symbols.insert(this->_symbols.begin(), symbol);
                break;
            case MIDDLE:
                this->_symbols.insert(this->_symbols.begin()+this->currentPosition-1, symbol);
                break;

        }
        std::cout << changed << " Character inserted" << std::endl;

    }
}

void MainEditor::notifyMove() {
    int position = this->ui->textArea->textCursor().position();
    std::cout << "Cursor moved at position" << position << std::endl;
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;

}

