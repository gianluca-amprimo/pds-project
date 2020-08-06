//
// Created by antonio on 25/04/20.
//

#ifndef EDITOR_SYMBOL_H
#define EDITOR_SYMBOL_H
#include <iostream>
#include <QVector>
#include <QtCore/QDataStream>
#include <QtGui/QTextCharFormat>

class Symbol {
private:
    QChar character;
    QVector<int> position;
    QString identifier;
    QTextCharFormat charFormat;

public:
    Symbol();
    Symbol(QChar character, QString identifier, QVector<int> position, QTextCharFormat charFormat);

    const QTextCharFormat &getCharFormat() const;
    const QString &getIdentifier() const;

    const QChar &getCharacter() const;
    void setCharacter(QChar character);

    const QVector<int> &getPosition() const;
    void setPosition(const QVector<int> &position);
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
