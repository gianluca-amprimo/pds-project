//
// Created by antonio on 08/08/20.
//

#include "Session.h"
#include <QCryptographicHash>

Session::Session(const QString &filename) : filename(filename) {
    QCryptographicHash md5(QCryptographicHash::Md5);
    QByteArray filenameBytes;
    QDataStream ds(&filenameBytes, QIODevice::WriteOnly);
    ds << filename;
    md5.addData(filenameBytes);

    editorPrefix = md5.result().toHex();
    editorCounter = 0;
}

void Session::setFilename(const QString &filename) {
    Session::filename = filename;
}

const QString &Session::getFilename() const {
    return filename;
}

void Session::removeSymbol(QString id) {
    if(!symbolsById.contains(id)){
        NonExistingSymbol nes;
        throw nes;
    }
    FracPosition position = symbolsById.value(id)->getPosition();
    symbolsById.remove(id);
    symbolsByPosition.remove(position);
}

void Session::addSymbol(Symbol& sym) {
    if(symbolsById.contains(sym.getIdentifier())){
        #if DEBUG
            qDebug() << "+++++++WARNING: SYMBOL WITH THIS IDENTIFIER EXISTS+++++++";
            qDebug() << sym.getIdentifier();
        #endif
    }
    Symbol* symbol = new Symbol(sym);
    std::shared_ptr<Symbol> shared_symbol(symbol);
    symbolsById.insert(sym.getIdentifier(), shared_symbol);
    symbolsByPosition.insert(sym.getPosition(), shared_symbol);
}

int Session::getEditorCounter() const {
    return editorCounter;
}

void Session::setEditorCounter(int editorCounter) {
    Session::editorCounter = editorCounter;
}

const QString &Session::getEditorPrefix() const {
    return editorPrefix;
}

void Session::setEditorPrefix(const QString &editorPrefix) {
    Session::editorPrefix = editorPrefix;
}

void Session::addUserToSession(QString username, QString editorId) {
    this->userEditorId.insert(username, editorId);
    this->editorCounter++;
}

QString Session::removeUserFromSession(const QString& username) {
    auto editorId = this->userEditorId.value(username);
    this->userEditorId.remove(username);
    this->userMap.remove(username);
    this->editorCounter--;
    return editorId;
}

const QHash<QString, std::shared_ptr<Symbol>> &Session::getSymbolsById() const {
    return this->symbolsById;
}

const QMap<FracPosition, std::shared_ptr<Symbol>> &Session::getSymbolsByPosition() const {
    return symbolsByPosition;
}

const void Session::removeBatchSymbol(QHash<QString, FracPosition>& symbolsPosition) {
    for(QString key : symbolsPosition.keys()){
        removeSymbol(key);
    }
}

void Session::changeSymbolFormat(QHash<QString, FracPosition> symbolsPosition, QTextCharFormat format) {
    for(QString id : symbolsPosition.keys()){
       this->symbolsById.value(id)->setCharFormat(format);
    }
}

