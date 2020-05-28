//
// Created by antonio on 25/04/20.
//

#include "Symbol.h"

#include <utility>

Symbol::Symbol(wchar_t character, std::wstring identifier, std::vector<int> position) : character(character),
                                                                                   identifier(std::move(identifier)),
                                                                                   position(std::move(position)) {}

wchar_t Symbol::getCharacter() const {
    return character;
}

void Symbol::setCharacter(wchar_t character) {
    Symbol::character = character;
}

const std::vector<int> &Symbol::getPosition() const {
    return position;
}

void Symbol::setPosition(const std::vector<int> &position) {
    Symbol::position = position;
}
