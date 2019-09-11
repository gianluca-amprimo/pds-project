//
// Created by Andrea Foderaro on 2019-09-03.
//

#ifndef ES3_CRDT_SYMBOL_H
#define ES3_CRDT_SYMBOL_H


#include <vector>

class Symbol {
private:
    char value;
    int idClient;
    int num;
    std::vector<int> pos;

public:
    Symbol(char value, int client, int num, std::vector<int> pos);

    char getValue() const;
    int getIdClient() const;
    int getNum() const;
    std::vector<int> &getPos();

};


#endif //ES3_CRDT_SYMBOL_H
