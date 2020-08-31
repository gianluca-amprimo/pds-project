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
    //Get symbols in the range
    QVector<Symbol> syms = getSymbolInRange(this->oldPosition, this->currentPosition);
    for(Symbol sym : syms){
        // obtain information about the symbol and perform consequent actions
        QString symId = sym.getIdentifier();
        FracPosition pos = sym.getPosition();
        QByteArray serializedSymID;
        QDataStream symbolStream(&serializedSymID, QIODevice::WriteOnly);

        symbolStream << symId;
        // remove the symbol from the queue
        this->_symbols.remove(pos);
        // send symbol id to delete
        emit symbolDeleted(serializedSymID);
        qDebug() << "Character deleted" ;
    }
}

void MyTextArea::deleteSelection() {
    qDebug() << "selection anchor " << this->anchor ;
    qDebug() << "selection position " << this->oldPosition ;

    QVector<Symbol> syms = getSymbolInRange(this->anchor, this->oldPosition);

    for(Symbol sym : syms){
        QString symId = sym.getIdentifier();
        FracPosition pos = sym.getPosition();
        QByteArray serializedSymID;
        QDataStream symbolStream(&serializedSymID, QIODevice::WriteOnly);

        symbolStream << symId;
        emit symbolDeleted(serializedSymID);
        this->_symbols.remove(pos);
    }
}

void MyTextArea::insertSymbol(QChar changed, int insertPosition) {

    this->charCounter++;

    // calcolo l'id rispetto al mio editor
    int localCharIdLen = std::to_wstring(this->charCounter).length();

    FracPosition position;
    QString localCharId  = QString(6-localCharIdLen, '0') + QString::number(charCounter);
    QString charId = this->thisEditorIdentifier + localCharId;

    // calcolo la posizione frazionaria
    // se il carattere è inserito in fondo basta solo vedere il primo intero della posizione frazionaria
    // se invece è in mezzo bisogna trovare l'intero

    // last
    if(insertPosition == this->_symbols.size()+1){ // +1 because QChar is already inserted
        lastPosition = insertPosition;
        qDebug() << "Inserting at the back";
        // take just the first number of the last element and add 1

        //position of last QChar
        // first letter inserted
        if (this->_symbols.empty()) {
            position = one;
        } else {
            // last letter inserted
            FracPosition last = this->_symbols.lastKey();
            position = last + one;
        }
    }else if(insertPosition == 1){
        qDebug() << "Inserting at the head";
        // retrieve prior first element
        FracPosition oldFirstPos = this->_symbols.firstKey();
        position = oldFirstPos.divideByTwo();


    }else if(insertPosition > 1 && insertPosition < this->_symbols.size()+1) {
        FracPosition prevPos;
        FracPosition nextPos;

        nextPos = this->_symbols.keys().at(insertPosition-1);
        prevPos = this->_symbols.keys().at(insertPosition-2);

        position = nextPos + prevPos;
        position = position.divideByTwo();
// this should be enough to consider both the comprehending symbols

    }

    // get wstring of fractionary position

    //FracPosition pos2("1");
    Symbol symbol(changed, charId, position, this->currentCharFormat());
    //Symbol symbol2(changed, charId, pos2, this->currentCharFormat());
    qDebug() << "Inserting symbol with id:" << symbol.getIdentifier() << ", char:" << symbol.getCharacter()
                                                                      << ", position" << symbol.getPosition().getStringPosition();

    this->_symbols.insert(symbol.getPosition(), symbol);
    //this->_symbols.insert(symbol2.getPosition(), symbol2);


    // finally, prepare message for the server


    QJsonObject message;
    message["header"] = "symbol";


    // signal will be caught by MainEditor which will wrap the symbol in a JSON message
    emit symbolReady(symbol);
}

void MyTextArea::inputMethodEvent(QInputMethodEvent *event) {
    QTextEdit::inputMethodEvent(event);
    int position = this->textCursor().position();
    this->oldPosition = this->currentPosition;
    this->currentPosition = position;

    //QChar character = event->commitString()[0].unicode();
    insertSymbol(QChar(), this->currentPosition);
}

/* this function should help in retrieving symbols
 * based on the cursor position in the editor
 */
const Symbol& MyTextArea::getSymbolFromPosition(int position) {
    /* si prende sempre il simbolo che sta dietro al cursore
     * quindi se la posizione passata è 1, si prende il primo simbolo
     */
    return this->_symbols[this->_symbols.keys().at(position-1)];
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

const QMap<FracPosition, Symbol> &MyTextArea::getSymbols() const {
    return _symbols;
}

void MyTextArea::addSymbolToList(Symbol sym) {
    this->_symbols.insert(sym.getPosition(), sym);
    QTextCursor cur = this->textCursor();
    cur.setPosition(this->getEditorPosition(sym.getPosition()));
    cur.insertText(QString(sym.getCharacter()), sym.getCharFormat());
}

int MyTextArea::getEditorPosition(const FracPosition& fp) {
    return this->_symbols.keys().indexOf(fp);
}

void MyTextArea::removeSymbolFromList(QString& symId, QString& fp) {
    FracPosition fracPos(fp);
    qDebug() << "Trying to delete character at frac position" << fracPos.getStringPosition();
    QTextCursor cur = this->textCursor();
    cur.setPosition(this->getEditorPosition(fracPos));
    cur.deleteChar();
    if(this->_symbols.value(fracPos).getIdentifier() == symId){
        this->_symbols.remove(fracPos);
    }
}

QVector<Symbol> MyTextArea::getSymbolInRange(int end1, int end2) {
    QVector<Symbol> symbolsInRange;

    if(end1 > end2){
        for(int i = end2+1; i <= end1; i++)
            symbolsInRange.push_back(getSymbolFromPosition(i));
    }else if(end2 > end1){
        for(int i = end1+1; i <= end2; i++)
            symbolsInRange.push_back(getSymbolFromPosition(i));
    }
    return symbolsInRange;
}





