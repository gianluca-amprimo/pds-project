//
// Created by Andrea Foderaro on 2019-09-03.
//

#ifndef ES3_CRDT_MESSAGE_H
#define ES3_CRDT_MESSAGE_H


#include <vector>

class Message {
    bool insert;                    // true --> insert || false --> delete
    int _siteId;
    int num;
    char value{};
    std::vector<int> pos;

public:
    Message(bool insert, int siteId, int num, char value, std::vector<int> pos);
    Message(bool insert, int siteId, int num);

    bool isInsert() const;
    int getSiteId() const;
    int getNum() const;
    char getValue() const;
    const std::vector<int> &getPos() const;
};


#endif //ES3_CRDT_MESSAGE_H
