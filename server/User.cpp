//
// Created by gianluca on 06/04/20.
//

#include "User.h"

User::User(QString username) : username(username) {}

User::~User() {

}

bool User::operator==(const User &rhs) const {
    return username == rhs.username;
}

bool User::operator<(const User &rhs) const {
    return username < rhs.username;
}

bool User::operator>(const User &rhs) const {
    return rhs < *this;
}

bool User::operator<=(const User &rhs) const {
    return !(rhs < *this);
}

bool User::operator>=(const User &rhs) const {
    return !(*this < rhs);
}

bool User::operator!=(const User &rhs) const {
    return !(rhs == *this);
}

const QString &User::getUsername() const {
    return username;
}
