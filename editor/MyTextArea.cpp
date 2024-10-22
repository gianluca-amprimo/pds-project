//
// Created by antonio on 04/05/20.
//

#include "MyTextArea.h"

#include <utility>


MyTextArea::MyTextArea(QWidget *parent) : QTextEdit(parent) {
    setAttribute(Qt::WA_InputMethodEnabled, false);
    this->charCounter = 0;
    this->currentPosition = 0;
    QObject::connect(this, &MyTextArea::currentCharFormatChanged, this, &MyTextArea::updateStyle);
}

MyTextArea::MyTextArea() : QTextEdit(nullptr) {
    this->charCounter = 0;
    this->currentPosition = 0;
    QObject::connect(this, &MyTextArea::currentCharFormatChanged, this, &MyTextArea::updateStyle);
}

const QString &MyTextArea::getThisEditorIdentifier() const {
    return thisEditorIdentifier;
}

void MyTextArea::setThisEditorIdentifier(const QString &thisEditorIdentifier) {
    MyTextArea::thisEditorIdentifier = thisEditorIdentifier;
}

void MyTextArea::keyPressEvent(QKeyEvent *e) {
    /* è importante che il cursore visivo sia sincronizzato il più possibile con la posizione
     * corrente di inserimento. Questo perchè l'utente deve sempre avere il controllo di dove sta inserendo.
     */

    if (KEY_IS_BACKSPACE(e)) {
        if (this->_symbols.empty()) {
            goto empty;
        }
        if (selectionMode) {

            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(
                    anchor, currentPosition);
            goto empty;
        }
        if (KEY_CTRL_IS_ON(e)) {
            QTextCursor begin = this->textCursor();
            begin.setPosition(this->currentPosition);
            begin.movePosition(QTextCursor::PreviousWord);
            deleteBatchChar(begin.position(), this->currentPosition);
          //  this->currentPosition = begin.position();
          // this->textCursor().setPosition(this->currentPosition);
        } else {
            deleteChar(this->currentPosition);
            // Non c'è bisogno di aggiornare la posizion
            // perchè i backspace non hanno un ordine prioritario
            // (il server riconosce se si sta provando a cancellare in una posizion
            // in cui non esiste un carattere)
        }
        empty:
        selectionMode = false;

    } else if (KEY_IS_A_CHAR(e) && !KEY_CTRL_IS_ON(e)) {
        QChar unicode = e->text()[0];
        if (selectionMode) {
            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(
                    anchor, currentPosition);
            #if DEBUG
                qDebug() << "substituting selection";
            #endif
            selectionMode = false;
            this->anchor = this->textCursor().anchor();
        }
        insertChar(unicode, this->currentPosition, this->textCursor().charFormat());
        this->currentPosition++;
        this->textCursor().setPosition(this->currentPosition);
        // TODO: qui c'è bisogno di aggiornare la posizione di inserimento perchè
        //  l'utente mentre è nel flusso di scrittura rischia di sbagliare se aspetta un messaggio
        //  dal server. Dato che vogliamo che l'utente comunque sappia dove sarà la prossima posizione di inserimento
        //  aggiorniamo subito anche il cursore in maniera che anche se non arriva la risposta dal server,
        //  saprà da dove inserire

    } else if (KEY_IS_ARROW(e) || KEY_IS_SELECT_ALL(e)) { // from now on, manage shortcuts
        QTextEdit::keyPressEvent(e);
        this->currentPosition = this->textCursor().position();
        if (this->textCursor().position() != this->textCursor().anchor()) {
            #if DEBUG
                qDebug() << "selection mode";
            #endif
            this->selectionMode = true;

        }
    } else if(KEY_IS_PASTE(e)){
        QTextEdit::keyPressEvent(e);
    } else if (KEY_IS_CUT(e)) {
        if (selectionMode) {
            this->clipboard->setText(this->textCursor().selectedText());
            this->anchor > this->currentPosition ? deleteBatchChar(currentPosition, anchor) : deleteBatchChar(
                    anchor, currentPosition);
            this->currentPosition = this->textCursor().selectionStart();
            this->textCursor().setPosition(this->currentPosition);
            this->selectionMode = false;
        }
    } else if (KEY_IS_COPY(e)) {
        this->clipboard->setText(this->textCursor().selectedText());
    } else if(KEY_IS_BOLD(e) || KEY_IS_ITALIC(e) || KEY_IS_UNDERLINE(e)){
        QTextCharFormat format = this->textCursor().charFormat();
        if(KEY_IS_BOLD(e)){
            this->isBold = !this->isBold;
            emit boldFormatActivate();
            format.setFontWeight(isBold ? QFont::Bold : QFont::Normal);
        }
        else if(KEY_IS_ITALIC(e)){
            this->isItalic = !this->isItalic;
            emit italicFormatActivate();
            format.setFontItalic(isItalic);
        }
        else if(KEY_IS_UNDERLINE(e)){
            this->isUnderline = !this->isUnderline;
            emit underlineFormatActivate();
            format.setFontUnderline(isUnderline);
        }
        this->textCursor().setCharFormat(format);
        qDebug() << "changing style";
        if(selectionMode){
            int begin = this->anchor;
            int end = this->currentPosition;
            changeSelectionFormat(begin, end, format);
        }
    }

    this->anchor = this->textCursor().anchor();
    // we have to catch the key in case it was a char to be inserted
}



void MyTextArea::inputMethodEvent(QInputMethodEvent *event) {
#if DEBUG
    qDebug() << event->commitString();
#endif

    QChar character = event->commitString()[0].unicode();
    insertChar(character, this->currentPosition, this->textCursor().charFormat());
    this->currentPosition++;
}

/* this function should help in retrieving symbols
 * based on the cursor position in the editor
 */
const Symbol &MyTextArea::getSymbolFromPosition(int position) {
    /* si prende sempre il simbolo che sta dietro al cursore
     * quindi se la posizione passata è 1, si prende il primo simbolo
     */
    return this->_symbols[this->_symbols.keys().at(position - 1)];
}

void MyTextArea::insertFromMimeData(const QMimeData *source) {
    this->currentPosition = this->textCursor().position();
    QVector<QTextCharFormat> formats;
    formats.push_back(this->textCursor().charFormat());
    insertBatchChar(source->text(), this->currentPosition, formats);
}

void MyTextArea::mouseReleaseEvent(QMouseEvent *e) {
    QTextEdit::mouseReleaseEvent(e);

    this->currentPosition = this->textCursor().position();
    this->anchor = this->textCursor().anchor();

    if (currentPosition != this->anchor) {
        #if DEBUG
            qDebug() << "selection mode";
        #endif
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
        this->anchor = other.anchor;
    }
    return *this;
}

const QMap<FracPosition, Symbol> &MyTextArea::getSymbols() const {
    return _symbols;
}

void MyTextArea::addSymbolToList(const Symbol& sym, QString color) {
    if(sym.getIdentifier().split("_")[0] != this->getThisEditorIdentifier()){
        if(getEditorPosition(sym.getPosition()) < this->currentPosition){
            this->currentPosition++;
        }
    }

    this->_symbols.insert(sym.getPosition(), sym);
    if(getEditorPosition(sym.getPosition()) < this->currentPosition){
        // this->currentPosition++;
    }

    QTextCursor cur = this->textCursor();
    cur.setPosition(this->getEditorPosition(sym.getPosition()));
    QTextCharFormat fmt = sym.getCharFormat();
    if (color != ""){
        QColor color_highlight{color};
        QBrush highlight{color_highlight};
        fmt.setBackground(highlight);

        // inserire un timer one shot
        QTimer::singleShot(800, this, [=]{MyTextArea::removeHighlight(sym);});
    }
    cur.insertText(QString(sym.getCharacter()), fmt);

    // TODO: bisogna gestire il fatto che il cursore aggiorni
    //  automaticamente la sua posizione
    //  quando vorremmo essere noi a controllarlo
    //  change position of the cursor
}

int MyTextArea::getEditorPosition(const FracPosition &fp) {
    return this->_symbols.keys().indexOf(fp);
}

void MyTextArea::removeSymbolFromList(QString &symId, QString &fp, QString color) {
    FracPosition fracPos(fp);
    if(symId.split("_")[0] != this->getThisEditorIdentifier()){
        if(getEditorPosition(fracPos) < this->currentPosition){
            this->currentPosition--;
        }
    }else if(symId.split("_")[0] == this->getThisEditorIdentifier()){
        this->currentPosition--;
        this->textCursor().setPosition(this->currentPosition);
    }

#if DEBUG
    qDebug() << "Trying to delete character at frac position" << fracPos.getStringPosition();
#endif

    QTextCursor cur = this->textCursor();
    cur.setPosition(this->getEditorPosition(fracPos));
    cur.setPosition(this->getEditorPosition(fracPos) + 1, QTextCursor::KeepAnchor);

    // highlight the color of the user who performed the operation
    if (color != ""){
        #if DEBUG
            qDebug() << "Background color is: " << color;
        #endif

        QTextCharFormat fmt{cur.charFormat()};
        QColor color_highlight{color};
        QBrush highlight{color_highlight};
        fmt.setBackground(highlight);
        cur.setCharFormat(fmt);
    }

    cur.setPosition(this->getEditorPosition(fracPos));
    cur.deleteChar();

    if (this->_symbols.value(fracPos).getIdentifier() == symId) {
        this->_symbols.remove(fracPos);
    }

}

QVector<Symbol> MyTextArea::getSymbolInRange(int end1, int end2) {
    QVector<Symbol> symbolsInRange;

    if (end1 > end2) {
        for (int i = end2 + 1; i <= end1; i++)
            symbolsInRange.push_back(getSymbolFromPosition(i));
    } else if (end2 > end1) {
        for (int i = end1 + 1; i <= end2; i++)
            symbolsInRange.push_back(getSymbolFromPosition(i));
    }
    return symbolsInRange;
}

void MyTextArea::insertChar(QChar unicode, int position, const QTextCharFormat& format) {
    /* here we simply prepare the message for the server */
    QJsonObject message;

    // serialize format
    QByteArray formatInBytes;
    QDataStream formatOut(&formatInBytes, QIODevice::WriteOnly);
    formatOut << format;

#if DEBUG
    qDebug() << "Sending char " << unicode << " at position " << position
             << " with format I:" << format.fontItalic() << "; U:" << format.fontUnderline() << "; B:" << (format.fontWeight() == QFont::Bold);
#endif

    message["header"] = SINGLE_CHAR_ADDITION;
    message["editorId"] = this->getThisEditorIdentifier();
    message["charId"] = "";
    message["unicode"] = QString(unicode);
    message["position"] = position;
    message["format"] = QLatin1String(formatInBytes.toBase64());

    emit charInserted(message);
}

void MyTextArea::deleteChar(int position) {
    // retrieve the symbol at the specified position
    if (position > this->_symbols.size()) {
        position = this->_symbols.size();
    }

    FracPosition keyPosition = this->_symbols.keys().at(position - 1);
#if DEBUG
    qDebug() << "Here frac position is:" << keyPosition.getStringPosition();
#endif
    Symbol symbol = this->_symbols.value(keyPosition);

    QJsonObject message;
    message["header"] = SINGLE_CHAR_DELETION;
    message["editorId"] = this->getThisEditorIdentifier();
    message["charId"] = symbol.getIdentifier();

#if DEBUG
    qDebug() << "char to be deleted is " << symbol.getIdentifier();
#endif

    emit charDeleted(message);
}

void MyTextArea::deleteBatchChar(int begin, int end) {
    QVector<Symbol> symbolsInRange = getSymbolInRange(begin, end);
    QVector<QString> idsInRange;
    this->currentPosition=end;
    for (Symbol sym : symbolsInRange) {
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

    for (int i = 0; i < text.length(); i++) {
        this->currentPosition++;
        QJsonObject character;
        QByteArray formatInBytes;

        // prepare id for char
        QChar unicode = text[i];

        int localCharIdLen = std::to_wstring(this->charCounter).length();
        QString localCharId = QString(6 - localCharIdLen, '0') + QString::number(charCounter);
        QString charId = this->thisEditorIdentifier + localCharId;

        // serialize format

        character["charId"] = charId;
        character["unicode"] = QString(unicode);
        character["position"] = position + i;
#if DEBUG
        qDebug() << "position is" << character["position"];
#endif
        character["format"] = QLatin1String(formatInBytes.toBase64());

        message.push_back(character);
    }

#if DEBUG
    qDebug() << "next insert will be at position" << this->currentPosition;
#endif

    emit batchCharInserted(message, std::move(formats));
}

int MyTextArea::getCurrentPosition() const {
    return this->currentPosition;
}

void MyTextArea::removeHighlight(const Symbol& sym) {
    QTextCursor cur = this->textCursor();
    // qDebug() << "Symbol selected " << sym.getCharacter() << " at position " << sym.getPosition().getStringPosition();
    cur.setPosition(this->getEditorPosition(sym.getPosition()));
    cur.setPosition(this->getEditorPosition(sym.getPosition()) + 1, QTextCursor::KeepAnchor);

    // qDebug() << cur.selectedText() << " char selected";

    // reset the symbol format
    QTextCharFormat fmt = sym.getCharFormat();
    fmt.clearBackground();
    cur.setCharFormat(fmt);
}

void MyTextArea::changeSelectionFormat(int begin, int end, QTextCharFormat format) {
    QVector<Symbol> symbolsInRange = getSymbolInRange(begin, end);
    QVector<QString> idsInRange;
    this->currentPosition=end;
    this->textCursor().setPosition(end);
    for (Symbol sym : symbolsInRange) {
        idsInRange.push_back(sym.getIdentifier());
    }

    QJsonObject message;
    QByteArray formatInBytes;
    QDataStream formatInByteStream(&formatInBytes, QIODevice::WriteOnly);
    QByteArray symbolsInByte;
    QDataStream symbolsInByteStream(&symbolsInByte, QIODevice::WriteOnly);

    symbolsInByteStream << idsInRange;
    formatInByteStream << format;
    message["header"] = CHANGE_CHAR_FORMAT;
    message["editorId"] = this->getThisEditorIdentifier();
    message["format"] = QLatin1String(formatInBytes.toBase64());
    message["idsToChange"] = QLatin1String(symbolsInByte.toBase64());

    emit formatCharChanged(message);

}

void MyTextArea::changeSymbolFormat(QString &symId, QString &fp, QTextCharFormat format) {
    FracPosition fracPos(fp);


    QTextCursor cur = this->textCursor();
    cur.setPosition(this->getEditorPosition(fracPos));
    cur.setPosition(this->getEditorPosition(fracPos) + 1, QTextCursor::KeepAnchor);

    cur.setCharFormat(format);
#if DEBUG
    qDebug() << "font weight is" << format.fontWeight();
#endif

    // highlight the color of the user who performed the operation
    //if (color != ""){
#if DEBUG
    //    qDebug() << "Background color is: " << color;
#endif

    //    QTextCharFormat fmt{cur.charFormat()};
    //    QColor color_highlight{color};
    //    QBrush highlight{color_highlight};
    //    fmt.setBackground(highlight);
    //    cur.setCharFormat(fmt);
    //}

    cur.setPosition(this->getEditorPosition(fracPos));

    if (this->_symbols.value(fracPos).getIdentifier() == symId) {
        Symbol sym = this->_symbols.value(fracPos);
        sym.setCharFormat(format);
        this->_symbols.remove(fracPos);
        this->_symbols.insert(fracPos, sym);
    }

}

const QMetaObject &MyTextArea::getStaticMetaObject() {
    return staticMetaObject;
}

bool MyTextArea::isBold1() const {
    return isBold;
}

void MyTextArea::setIsBold(bool isBold) {
    MyTextArea::isBold = isBold;
}

bool MyTextArea::isItalic1() const {
    return isItalic;
}

void MyTextArea::setIsItalic(bool isItalic) {
    MyTextArea::isItalic = isItalic;
}

bool MyTextArea::isUnderline1() const {
    return isUnderline;
}

void MyTextArea::setIsUnderline(bool isUnderline) {
    MyTextArea::isUnderline = isUnderline;
}

void MyTextArea::updateStyle(const QTextCharFormat &f) {
    this->isItalic = f.fontItalic();
    this->isUnderline = f.fontUnderline();
    this->isBold = ( f.fontWeight() == QFont::Bold);
}





