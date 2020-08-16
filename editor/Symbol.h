//
// Created by antonio on 25/04/20.
//

#ifndef EDITOR_SYMBOL_H
#define EDITOR_SYMBOL_H
#include <iostream>
#include <QVector>
#include <QtCore/QDataStream>
#include <QtGui/QTextCharFormat>
#include "FracPosition.h"

class Symbol {
private:
    QChar character;
    FracPosition position;
    QString identifier;
    QTextCharFormat charFormat;

public:
    Symbol();
    Symbol(QChar character, QString identifier, FracPosition position, QTextCharFormat charFormat);
    Symbol(const Symbol& sym);

    const QTextCharFormat &getCharFormat() const;
    const QString &getIdentifier() const;

    const QChar &getCharacter() const;

    const FracPosition getPosition() const;

    void setCharacter(QChar character);

    QDataStream& serialize(QDataStream& out) const;
    QDataStream& deserialize(QDataStream& in);

    friend QDataStream& operator<<(QDataStream& out, Symbol const& sym) {
        return sym.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, Symbol& sym) {
        return  sym.deserialize(in);
    }
};


#endif //EDITOR_SYMBOL_H
