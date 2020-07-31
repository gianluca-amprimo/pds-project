//
// Created by antonio on 25/04/20.
//

#include "Symbol.h"
#include <utility>

Symbol::Symbol() : character(QChar()), identifier(QString()), position(QVector<int>()) {}

Symbol::Symbol(QChar character) : character(character), identifier(QString()), position(QVector<int>()) {}

Symbol::Symbol(QChar character, QString identifier, QVector<int> position) : character(character),
                                                                             identifier(std::move(identifier)),
                                                                             position(std::move(position)) {}

const QChar Symbol::getCharacter() const {
    return character;
}

void Symbol::setCharacter(wchar_t character) {
    this->character = character;
}

const QVector<int>& Symbol::getPosition() const {
    return position;
}

void Symbol::setPosition(const QVector<int>& position) {
    this->position = position;
}

QDataStream& Symbol::serialize(QDataStream& out) const {
    out << character;
    out << position;
    out << identifier;
    return out;
}

QDataStream& Symbol::deserialize(QDataStream& in) {
    in >> character;
    in >> position;
    in >> identifier;
    return in;
}


