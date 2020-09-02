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

    // we have to catch the key in case it was a char to be inserted
    if(KEY_IS_BACKSPACE){

        if(this->_symbols.empty()) goto empty;
        if (selectionMode){
            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(anchor, currentPosition);
            this->currentPosition = this->textCursor().position();
            goto empty;
        }
        if (KEY_CTRL_IS_ON){
            QTextCursor begin = this->textCursor();
            begin.movePosition(QTextCursor::PreviousWord);
            deleteBatchChar(begin.position(), this->currentPosition);
            this->currentPosition = begin.position();
        }
        else{
            deleteChar(this->currentPosition);
            this->currentPosition--;
        }
        empty:
        selectionMode = false;

    } else if(KEY_IS_A_CHAR && !KEY_CTRL_IS_ON) {
        QChar unicode = e->text()[0];
        if (selectionMode) {
            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(anchor, currentPosition);
            this->currentPosition = this->textCursor().position();
            qDebug() << "substituting selection";
            selectionMode = false;
            this->anchor = this->textCursor().anchor();
        }
        insertChar(unicode, this->currentPosition, this->textCursor().charFormat());
        this->currentPosition++;

    }else if(KEY_IS_ARROW || KEY_IS_SELECT_ALL || KEY_IS_PASTE){ // from now on, manage shortcuts
        QTextEdit::keyPressEvent(e);
        this->currentPosition = this->textCursor().position();
        if (this->textCursor().position() != this->textCursor().anchor()) {
            qDebug() << "selection mode";
            this->selectionMode = true;

        }
    }
    else if(KEY_IS_CUT){
        if (selectionMode){
            this->clipboard->setText(this->textCursor().selectedText());
            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(anchor, currentPosition);
            this->currentPosition = this->textCursor().selectionStart();
            this->selectionMode = false;
        }

    }else if(KEY_IS_COPY){
        this->clipboard->setText(this->textCursor().selectedText());
    }



//this->textCursor().setPosition(this->textCursor().position()+1);
    // we have to manage selection
    this->anchor = this->textCursor().anchor();
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
        //emit charDeleted(serializedSymID);
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
        //emit symbolDeleted(serializedSymID);
        this->_symbols.remove(pos);
    }
}

void MyTextArea::inputMethodEvent(QInputMethodEvent *event) {
    QChar character = event->commitString()[0].unicode();
    insertChar(character, this->currentPosition, this->textCursor().charFormat());
    this->currentPosition++;
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

    this->currentPosition = this->textCursor().position();
    int pasteLength = source->text().length();
    qDebug() << "Pastelength is: " << pasteLength ;

    for(int i = 0; i < pasteLength; i++){
        QChar character = this->toPlainText().toStdWString()[this->oldPosition+i];
        QVector<QTextCharFormat> formats;
        formats.push_back(this->textCursor().charFormat());
        insertBatchChar(source->text(), this->currentPosition, formats);
    }
    this->pasting = false;

}

void MyTextArea::mouseReleaseEvent(QMouseEvent *e) {
    QTextEdit::mouseReleaseEvent(e);

    this->currentPosition = this->textCursor().position();
    this->anchor = this->textCursor().anchor();

    if(currentPosition != this->anchor){
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
    // bisogna gestire il fatto che il cursore aggiorni automaticamente la sua posizione
    // quando vorremmo essere noi a controllarlo
    this->textCursor().setPosition(this->textCursor().position()-1);

    // change position of the cursor
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

void MyTextArea::insertChar(QChar unicode, int position, QTextCharFormat format) {

    /* here we simply prepare the message for the server */

    QJsonObject message;
    QByteArray formatInBytes;

    // prepare id for char

    this->charCounter++;
    int localCharIdLen = std::to_wstring(this->charCounter).length();
    QString localCharId  = QString(6-localCharIdLen, '0') + QString::number(charCounter);
    QString charId = this->thisEditorIdentifier + localCharId;

    // serialize format
    QDataStream formatOut(&formatInBytes, QIODevice::WriteOnly);
    formatOut.setVersion(QDataStream::Qt_4_0);
    formatOut << format;

    message["header"] = SINGLE_CHAR_ADDITION;
    message["editorId"] = this->getThisEditorIdentifier();
    message["charId"] = charId;
    message["unicode"] = QString(unicode);
    message["position"] = position;
    qDebug() << "position is" << message["position"];
    message["format"] = QLatin1String(formatInBytes.toBase64());

    emit charInserted(message);

}

void MyTextArea::deleteChar(int position) {
    // retrieve the symbol at the specified position

    FracPosition keyPosition = this->_symbols.keys().at(position-1);
    qDebug() << "Here frac position is:" << keyPosition.getStringPosition();
    Symbol symbol = this->_symbols.value(keyPosition);

    QJsonObject message;
    message["header"] = SINGLE_CHAR_DELETION;
    message["editorId"] = this->getThisEditorIdentifier();
    message["charId"] = symbol.getIdentifier();

    qDebug() << "char to be deleted is " << symbol.getIdentifier();

    emit charDeleted(message);
}

void MyTextArea::deleteBatchChar(int begin, int end) {
    QVector<Symbol> symbolsInRange = getSymbolInRange(begin, end);
    QVector<QString> idsInRange;
    for(Symbol sym : symbolsInRange){
        idsInRange.push_back(sym.getIdentifier());
    }

    QJsonObject message;
    QByteArray symbolsInByte;
    QDataStream symbolsInByteStream(&symbolsInByte, QIODevice::WriteOnly);

    symbolsInByteStream << idsInRange;
    message["header"] = BATCH_CHAR_DELETION;
    message["editorId"] = this->getThisEditorIdentifier();
    message["idsToDelete"] = QLatin1String(symbolsInByte.toBase64());

    emit batchCharDelete(message);

}

void MyTextArea::insertBatchChar(QString text, int position, QVector<QTextCharFormat> formats) {
    // first of all, take string and create a vector of messages
    QJsonArray message;

    for(int i = 0; i < text.length(); i++){
        QJsonObject character;
        QByteArray formatInBytes;

        // prepare id for char
        QChar unicode = text[i];
        this->charCounter++;
        int localCharIdLen = std::to_wstring(this->charCounter).length();
        QString localCharId  = QString(6-localCharIdLen, '0') + QString::number(charCounter);
        QString charId = this->thisEditorIdentifier + localCharId;

        // serialize format

        character["charId"] = charId;
        character["unicode"] = QString(unicode);
        character["position"] = position+i;
        qDebug() << "position is" << character["position"];
        character["format"] = QLatin1String(formatInBytes.toBase64());

        message.push_back(character);
    }

    emit batchCharInserted(message, formats);
}





