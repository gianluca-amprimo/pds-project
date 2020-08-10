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
}


void Session::setFilename(const QString &filename) {
    Session::filename = filename;
}

const QString &Session::getFilename() const {
    return filename;
}

void Session::removeSymbol(QString id) {
    symbols.remove(id);
}

void Session::addSymbol(Symbol sym) {
    QString id = sym.getIdentifier();
    symbols.insert(id, sym);
}

const QMap<QString, &Session::getSymbols() const {
    return symbols;
}

void Session::setSymbols(const QMap<QString, &symbols) {
    Session::symbols = symbols;
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
