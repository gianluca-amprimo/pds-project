//
// Created by Andrea Foderaro on 2019-09-03.
//

#include <utility>

#include "Message.h"

Message::Message(bool insert, int siteId, int num, char value, std::vector<int> pos) : insert(insert),
                                                                                              _siteId(siteId), num(num),
                                                                                              value(value), pos(std::move(pos)) {}

Message::Message(bool insert, int siteId, int num) : insert(insert), _siteId(siteId), num(num) {}

bool Message::isInsert() const {
    return insert;
}

int Message::getSiteId() const {
    return _siteId;
}

int Message::getNum() const {
    return num;
}

char Message::getValue() const {
    return value;
}

const std::vector<int> &Message::getPos() const {
    return pos;
}
