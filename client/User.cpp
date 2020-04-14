//
// Created by gianluca on 06/04/20.
//

#include "User.h"

User::User(QString username, QPixmap propic, QString name, QString surname): username(username), propic(propic), name(name), surname(surname) {}

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

const QPixmap &User::getPropic() const {
    return propic;
}

const QString &User::getName() const {
    return name;
}

void User::setName(const QString &name) {
    User::name = name;
}

const QString &User::getSurname() const {
    return surname;
}

void User::setSurname(const QString &surname) {
    User::surname = surname;
}

void User::setPropic(const QPixmap &propic) {
    User::propic = propic;
}
