//
// Created by Andrea Foderaro on 2019-09-03.
//

#include "NetworkServer.h"

int NetworkServer::connect(SharedEditor *sharedEditor) {
    int idEditor = 0;

    while(idEditor == 0){               // genero id univoco per sharedEditor
        idEditor = rand();

        auto it = std::find(this->idEditors.begin(), this->idEditors.end(), idEditor);
        if(it != this->idEditors.end())
            idEditor = 0;
    }

    this->idEditors.push_back(idEditor);
    this->SEditors.push_back(sharedEditor);
    return idEditor;
}

int NetworkServer::disconnect(SharedEditor *sharedEditor) {
    this->SEditors.erase(std::find(this->SEditors.begin(), this->SEditors.end(), sharedEditor));
    return 0;
}

void NetworkServer::send(const Message &m) {
    this->messages.push(m);
}

void NetworkServer::dispatchMessages() {
    while (!this->messages.empty()){
        Message m = this->messages.front();
        for (auto editor : this->SEditors){
            if (m.isInsert() && m.getSiteId() != editor->getSiteId())
                editor->process(m);
            else if(!m.isInsert() && m.getSiteId() == editor->getSiteId())
                editor->process(m);
        }
        this->messages.pop();
    }
}
