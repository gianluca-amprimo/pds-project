//
// Created by Andrea Foderaro on 2019-09-03.
//

#include <utility>

#include "Symbol.h"

Symbol::Symbol(char value, int client, int num, std::vector<int> pos) {
    this->value = value;
    this->idClient = client;
    this->num = num;
    this->pos = std::move(pos);
}

char Symbol::getValue() const {
    return value;
}

int Symbol::getIdClient() const {
    return idClient;
}

int Symbol::getNum() const {
    return num;
}

std::vector<int> &Symbol::getPos() {
    return pos;
}