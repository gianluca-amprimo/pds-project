//
// Created by antonio on 25/04/20.
//

#include "Symbol.h"

#include <utility>

Symbol::Symbol(QChar character, QString identifier, FracPosition position, QTextCharFormat charFormat) : character(character),
                                                                                   identifier(std::move(identifier)),
                                                                                   position(std::move(position)),
                                                                                   charFormat(charFormat){}

void Symbol::setCharacter(QChar character) {
    Symbol::character = character;
}


Symbol::Symbol() {
}

QDataStream& Symbol::serialize(QDataStream& out) const {
    out << character;
    out << position;
    out << identifier;
    out << charFormat;
    return out;
}

QDataStream& Symbol::deserialize(QDataStream& in) {
    in >> character;
    in >> position;
    in >> identifier;
    in >> charFormat;
    return in;
}

const QString &Symbol::getIdentifier() const {
    return identifier;
}

const QTextCharFormat &Symbol::getCharFormat() const {
    return charFormat;
}

const QChar &Symbol::getCharacter() const {
    return character;
}

const FracPosition Symbol::getPosition() const {
    return position;
}



