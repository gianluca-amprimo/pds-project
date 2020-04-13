//
// Created by Andrea Foderaro on 2019-09-03.
//

#ifndef ES3_CRDT_NETWORKSERVER_H
#define ES3_CRDT_NETWORKSERVER_H

#include <vector>
#include <queue>

#include "SharedEditor.h"
#include "Message.h"

class SharedEditor;

class NetworkServer {                               // simula il comportamento di un sistema di rete
private:
    std::vector<SharedEditor*> SEditors;
    std::vector<int> idEditors;
    std::queue<Message> messages;

public:
    int connect(SharedEditor* sharedEditor);
    int disconnect(SharedEditor* sharedEditor);

    void send(const Message& m);
    void dispatchMessages();
};


#endif //ES3_CRDT_NETWORKSERVER_H
