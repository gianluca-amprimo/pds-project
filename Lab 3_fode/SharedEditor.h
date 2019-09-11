//
// Created by Andrea Foderaro on 2019-09-03.
//

#ifndef ES3_CRDT_SHAREDEDITOR_H
#define ES3_CRDT_SHAREDEDITOR_H


#include "NetworkServer.h"
#include "Message.h"
#include "Symbol.h"

class NetworkServer;

class SharedEditor {
private:
    NetworkServer& _server;
    int _siteId{};
    std::vector<Symbol> _symbols;
    int _counter;

public:
    SharedEditor() = delete;
    SharedEditor(NetworkServer& server);

    void localInsert(int index, char value);
    void localErase(int index);

    void process(const Message& m);
    std::string to_string();

    int getSiteId() const;

};


#endif //ES3_CRDT_SHAREDEDITOR_H
