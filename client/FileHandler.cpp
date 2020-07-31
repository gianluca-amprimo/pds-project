//
// Created by fra on 31/07/20.
//

#include "FileHandler.h"

QDataStream& FileHandler::deserialize(QDataStream& in) {
    in >> listOfSymbols;
    return in;
}

QDataStream& FileHandler::serialize(QDataStream& out) const {
    out << listOfSymbols;
    return out;
}

void FileHandler::append(Symbol sym) {
    this->listOfSymbols.append(sym);
}


