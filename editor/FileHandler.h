//
// Created by fra on 31/07/20.
//

#ifndef SERVER_FILEHANDLER_H
#define SERVER_FILEHANDLER_H


#include "Symbol.h"

class FileHandler {
private:
    QList<Symbol> listOfSymbols;

public:
    void append(Symbol sym);

    QDataStream& serialize(QDataStream& out) const;

    QDataStream& deserialize(QDataStream& in);

    friend QDataStream& operator<<(QDataStream& out, FileHandler const& fh) {
        return fh.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, FileHandler& fh) {
        return fh.deserialize(in);
    }

    friend std::ostream& operator<<(std::ostream& out, FileHandler& fh) {
        // print something from v to str, e.g: Str << v.getX();
        for(Symbol sym : fh.listOfSymbols){
            out << sym.getCharacter().toLatin1();
        }
        return out;
    }
};


#endif //SERVER_FILEHANDLER_H
