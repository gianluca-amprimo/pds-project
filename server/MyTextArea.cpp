//
// Created by antonio on 04/05/20.
//

#include "MyTextArea.h"

MyTextArea::MyTextArea(QWidget *parent) : QTextEdit(parent) {
    setAttribute(Qt::WA_InputMethodEnabled, true);
    this->charCounter = 0;
    this->currentPosition = 0;
    this->oldPosition = 0;
}

MyTextArea::MyTextArea() : QTextEdit(nullptr) {
    this->charCounter = 0;
    this->currentPosition = 0;
    this->oldPosition = 0;
}

const QString &MyTextArea::getThisEditorIdentifier() const {
    return thisEditorIdentifier;
}

void MyTextArea::setThisEditorIdentifier(const QString &thisEditorIdentifier) {
    MyTextArea::thisEditorIdentifier = thisEditorIdentifier;
}

void MyTextArea::keyPressEvent(QKeyEvent *e) {
    QTextEdit::keyPressEvent(e);
    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;

    // we have to manage selection
    qDebug() << "Anchor: " << this->textCursor().anchor() ;
    qDebug() << "Position: " << this->textCursor().position() ;
    if(this->textCursor().position() != this->textCursor().anchor()){
        qDebug() << "selection mode" ;
        this->selectionMode = true;

    }

    if(e->key() == Qt::Key_Backspace || (e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_X)) {
        if (selectionMode) deleteSelection();
        else deleteSymbol();
        selectionMode = false;
        this->anchor = this->textCursor().anchor();
    }else if((e->key() >= Qt::Key_Space && e->key() <= Qt::Key_ydiaeresis || e->key() == Qt::Key_Return) &&
            !((e->key() == Qt::Key_V ||
               e->key() == Qt::Key_C || // taking care of keyboard shortcuts
               e->key() == Qt::Key_X ||
               e->key() == Qt::Key_A ) && e->modifiers() & Qt::ControlModifier )) {
        if (selectionMode) {
            deleteSelection();
            qDebug() << "substituting selection" ;
            deleteSelection();
            selectionMode = false;
            this->anchor = this->textCursor().anchor();
        }
        QChar changed;
        changed = toPlainText().toStdWString()[this->currentPosition - 1];
        insertSymbol(changed, this->currentPosition);
    }else{
        this->anchor = this->textCursor().anchor();
    }
}

void MyTextArea::deleteSymbol() {
    for(int i = 1; i <= this->oldPosition-this->currentPosition; i++){
        qDebug() << "Character deleted" ;
        this->_symbols.erase(this->_symbols.begin()+this->oldPosition-i);
    }
}

void MyTextArea::deleteSelection() {
    qDebug() << "selection anchor " << this->anchor ;
    qDebug() << "selection position " << this->oldPosition ;
    if(this->anchor > this->oldPosition){
        for(int i = 1; i <= this->anchor-this->oldPosition; i++){
            qDebug() << "Character deleted" ;
            this->_symbols.erase(this->_symbols.begin()+this->anchor-i);
        }
    }else{
        for(int i = 1; i <= this->oldPosition-this->anchor; i++){
            qDebug() << "Character deleted" ;
            this->_symbols.erase(this->_symbols.begin()+this->oldPosition-i);

        }
    }
}

void MyTextArea::insertSymbol(QChar changed, int insertPosition) {

    this->charCounter++;
    int insertType;
    qDebug() << "Insert position is: " << insertPosition ;

    // calcolo l'id rispetto al mio editor
    int localCharIdLen = std::to_wstring(this->charCounter).length();
    QVector<int> position;
    QString localCharId  = QString(6-localCharIdLen, '0') + QString::number(charCounter);
    QString charId = this->thisEditorIdentifier + localCharId;
    qDebug() << "char has id " << charId ;

    // calcolo la posizione frazionaria
    // se il carattere è inserito in fondo basta solo vedere il primo intero della posizione frazionaria
    // se invece è in mezzo bisogna trovare l'intero

    // last
    if(insertPosition == this->_symbols.size()+1){ // +1 because QChar is already inserted
        lastPosition = insertPosition;
        insertType = BACK;
        qDebug() << "Inserting at the back" ;
        // take just the first number of the last element and add 1
        QVector<int> fracPosition;
        int last;

        //position of last QChar
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
    }else if(insertPosition == 1){
        insertType = HEAD;
        // retrieve prior first element
        qDebug() << "Inserting at the head" ;
        QVector<int> oldFirstPos = this->_symbols.front().getPosition();
        int numDecimals = oldFirstPos.size();
        for(int i = 0; i < numDecimals; i++)
            position.push_back(0);

        position.push_back(5);

    }else if(insertPosition > 1 && insertPosition < this->_symbols.size()+1) {
        insertType = MIDDLE;
        qDebug() << L"Inserting in the middle";
        QVector<int> prevPos;
        QVector<int> nextPos;
        prevPos = this->_symbols.at(insertPosition - 2).getPosition();
        nextPos = this->_symbols.at(insertPosition-1).getPosition();

        // this should be enough to consider both the comprehending symbols

        position = prevPos;
        position.push_back(5);
        QString strPosition = "";
        for(int prevPo : prevPos){
            strPosition += prevPo + '0';
        }
        qDebug() << L"Prev pos is: " << strPosition ;
        strPosition = "";
        for(int nextPo : nextPos){
            strPosition += nextPo + '0';
        }
        qDebug() << "Next pos is: " << strPosition ;
        if (position == nextPos) {
            position.pop_back();
            position.push_back(0);
            position.push_back(5);
        }
    }

    // get wstring of fractionary position

    QString strPosition = "";
    for(int i : position){
        strPosition += i + '0';
    }

    qDebug() << "fractionary position " << strPosition;

    Symbol symbol(changed, charId, position, this->currentCharFormat());
    switch(insertType){
        case BACK:
            this->_symbols.push_back(symbol);
            break;
        case HEAD:
            this->_symbols.insert(0, symbol);
            break;
        case MIDDLE:
            this->_symbols.insert(insertPosition-1, symbol);
            break;
        default:
            break;

    }
    qDebug() << changed << L" Character inserted" ;
    qDebug() ;
}

void MyTextArea::inputMethodEvent(QInputMethodEvent *event) {
    QTextEdit::inputMethodEvent(event);
    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;

    //QChar character = event->commitString()[0].unicode();

    insertSymbol(QChar(), this->currentPosition);
}

void MyTextArea::insertFromMimeData(const QMimeData *source) {
    QTextEdit::insertFromMimeData(source);
    this->repaint();

    this->oldPosition = this->currentPosition;
    this->currentPosition = this->textCursor().position();
    int pasteLength = this->currentPosition-this->oldPosition;
    qDebug() << "Pastelength is: " << pasteLength ;

    this->pasting = true;
    for(int i = 0; i < pasteLength; i++){
        QChar character = this->toPlainText().toStdWString()[this->oldPosition+i];
        qDebug() << "Inserting pasted symbol" ;
        insertSymbol(character, this->oldPosition+i+1);
    }
    this->pasting = false;

}

void MyTextArea::mouseReleaseEvent(QMouseEvent *e) {
    QTextEdit::mouseReleaseEvent(e);

    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;
    this->anchor = this->textCursor().anchor();

    if(position != this->anchor){
        qDebug() << "selection mode" ;
        this->selectionMode = true;

    }
}

QVector<Symbol> MyTextArea::get_symbols() {
    return this->_symbols;
}



QDataStream &MyTextArea::serialize(QDataStream &out) const {
    out << thisEditorIdentifier;
    out << charCounter;
    out << _symbols;
    out << currentPosition;
    out << oldPosition;
    out << lastPosition;
    out << selectionMode;
    out << pasting;
    out << anchor;
    return out;
}

QDataStream &MyTextArea::deserialize(QDataStream &in) {
    in >> thisEditorIdentifier;
    in >> charCounter;
    in >> _symbols;
    in >> currentPosition;
    in >> oldPosition;
    in >> lastPosition;
    in >> selectionMode;
    in >> pasting;
    in >> anchor;
    return in;
}

MyTextArea &MyTextArea::operator=(const MyTextArea &other) {
    if (this != &other) { // protect against invalid self-assignment
        this->charCounter = other.charCounter;
        this->_symbols = other._symbols;
        this->currentPosition = other.currentPosition;
        this->oldPosition = other.oldPosition;
        this->lastPosition = other.lastPosition;
        this->selectionMode = other.selectionMode;
        this->pasting = other.pasting;
        this->anchor = other.anchor;
    }
    return *this;
}




