//
// Created by antonio on 04/05/20.
//

#include "MyTextEditor.h"

MyTextEditor::MyTextEditor(QWidget *parent) : QTextEdit(parent) {

    setAttribute(Qt::WA_InputMethodEnabled, true);
    this->charCounter = 0;
    this->currentPosition = 0;
    this->oldPosition = 0;
}



const std::wstring &MyTextEditor::getThisEditorIdentifier() const {
    return thisEditorIdentifier;
}

void MyTextEditor::setThisEditorIdentifier(const std::wstring &thisEditorIdentifier) {
    MyTextEditor::thisEditorIdentifier = thisEditorIdentifier;
}


void MyTextEditor::keyPressEvent(QKeyEvent *e) {
    QTextEdit::keyPressEvent(e);
    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;
    wchar_t changed;
    changed = toPlainText().toStdWString()[this->currentPosition-1];

    if(e->key() == Qt::Key_Backspace){
        deleteSymbol();
    }else if(e->key() >= Qt::Key_Space && e->key() <= Qt::Key_ydiaeresis){
        insertSymbol(changed);
    }
}

void MyTextEditor::deleteSymbol() {
    for(int i = 1; i <= this->oldPosition-this->currentPosition; i++){
        std::wcout << "Character deleted" << std::endl;
        this->_symbols.erase(this->_symbols.begin()+this->oldPosition-i);
    }
}

void MyTextEditor::insertSymbol(wchar_t changed) {

    this->charCounter++;
    int insertType;

    // calcolo l'id rispetto al mio editor
    int localCharIdLen = std::to_wstring(this->charCounter).length();
    std::vector<int> position;
    std::wstring localCharId  = std::wstring(6-localCharIdLen, '0') + std::to_wstring(charCounter);
    std::wstring charId = this->thisEditorIdentifier + localCharId;
    std::wcout << "char has id " << charId << std::endl;
    int lastPosition = 0;

    // calcolo la posizione frazionaria
    // se il carattere è inserito in fondo basta solo vedere il primo intero della posizione frazionaria
    // se invece è in mezzo bisogna trovare l'intero

    // last
    if(this->currentPosition > lastPosition){ // +1 because wchar_t is already inserted
        lastPosition = this->currentPosition;
        insertType = BACK;
        std::wcout << "Inserting at the back" << std::endl;
        // take just the first number of the last element and add 1
        std::vector<int> fracPosition;
        int last;

        //position of last wchar_t
        // first letter inserted
        if (this->_symbols.empty()) {
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
        std::wcout << "Inserting at the head" << std::endl;
        std::vector<int> oldFirstPos = this->_symbols.front().getPosition();
        int numDecimals = oldFirstPos.size();
        for(int i = 0; i < numDecimals; i++)
            position.push_back(0);

        position.push_back(5);

    }else if(this->currentPosition > 1 && this->currentPosition < this->_symbols.size()+1) {
        insertType = MIDDLE;
        std::wcout << L"Inserting in the middle" << std::endl;
        std::vector<int> prevPos = this->_symbols.at(this->currentPosition - 2).getPosition();
        std::vector<int> nextPos = this->_symbols.at(this->currentPosition-1).getPosition();

        // this should be enough to consider both the comprehending symbols

        position = prevPos;
        position.push_back(5);
        std::wstring strPosition = L"";
        for(int prevPo : prevPos){
            strPosition += prevPo + '0';
        }
        std::wcout << L"Prev pos is: " << strPosition << std::endl;
        strPosition = L"";
        for(int nextPo : nextPos){
            strPosition += nextPo + '0';
        }
        std::wcout << "Next pos is: " << strPosition << std::endl;
        if (position == nextPos) {
            position.pop_back();
            position.push_back(0);
            position.push_back(5);
        }
    }

    // get wstring of fractionary position

    std::wstring strPosition = L"";
    for(int i : position){
        strPosition += i + '0';
    }

    std::wcout << L"fractionary position " << strPosition  << std::endl;

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
        default:
            break;

    }
    std::wcout << changed << L" Character inserted" << std::endl;
}

void MyTextEditor::inputMethodEvent(QInputMethodEvent *event) {
    QTextEdit::inputMethodEvent(event);
    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;

    wchar_t character = event->commitString()[0].unicode();

    insertSymbol(character);
}

