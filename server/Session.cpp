//
// Created by antonio on 08/08/20.
//

#include "Session.h"
#include <QCryptographicHash>

Session::Session(const QString &filename) : filename(filename) {
    QCryptographicHash md5(QCryptographicHash::Md5);
    QByteArray filenameBytes;
    QDataStream ds(filenameBytes);
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
    symbols.remove(id);
    /*TODO: tell other editors to remove symbol*/
}

void Session::addSymbol(Symbol& sym) {
    symbols.insert(sym.getIdentifier(), sym);

    /*TODO: dispatch message to other editors*/
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

void Session::addUserToSession(User *u) {
    this->connectedEditors.insert(u->getEditorId(), u);
    this->editorCounter++;
}

// removing user from session
void Session::removeUserFromSession(User *u) {
    this->connectedEditors.remove(u->getEditorId());
    this->editorCounter--;
}

const QHash<QString, Symbol> &Session::getSymbols() const {
    return this->symbols;
}
