//
// Created by antonio on 03/09/20.
//

#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H

#include <QtCore/QException>

class NonExistingSymbol : public QException{
    void raise() const override { throw *this; }
    NonExistingSymbol *clone() const override { return new NonExistingSymbol(*this); }

};
#endif //SERVER_EXCEPTIONS_H
