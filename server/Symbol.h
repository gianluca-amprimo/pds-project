//
// Created by antonio on 25/04/20.
//

#ifndef SERVER_SYMBOL_H
#define SERVER_SYMBOL_H
#include <iostream>
#include <vector>
#include <QtCore/QDataStream>
#include <QtCore/QVector>


class Symbol {
private:


    QChar character;
    QVector<int> position;
    QString identifier;
public:
    Symbol();
    Symbol(QChar character);

    Symbol(QChar character, QString identifier, QVector<int> position);

    const QChar getCharacter() const;

    void setCharacter(wchar_t character);

    const QVector<int>& getPosition() const;

    void setPosition(const QVector<int>& position);

    QDataStream& serialize(QDataStream& out) const;
    QDataStream& deserialize(QDataStream& in);

    friend QDataStream& operator<<(QDataStream& out, Symbol const& sym) {
        return sym.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, Symbol& sym) {
        return  sym.deserialize(in);
    }
};


#endif //SERVER_SYMBOL_H
