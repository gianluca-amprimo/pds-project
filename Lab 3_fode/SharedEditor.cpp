//
// Created by Andrea Foderaro on 2019-09-03.
//

#include <iostream>

#include "SharedEditor.h"

SharedEditor::SharedEditor(NetworkServer &server):_server(server) {
    this->_siteId = this->_server.connect(this);
    this->_counter = 0;
}

void SharedEditor::localInsert(int index, char value) {
    std::vector<int> pre, current, pos;
    bool flag = false;

    // generate pre and current
    if (index > 0 && index < this->_symbols.size()){
        pre = this->_symbols.at(index - 1).getPos();
        current = this->_symbols.at(index).getPos();

        // generate vector for position
        for (int i = 0 ; i < pre.size() && i < current.size() ; i++ ){
            if(pre.at(i) == current.at(i)) {
                pos.push_back(pre.at(i));
            } else {
                if ((pre.at(i) - current.at(i)) > 2) {
                    pos.push_back((pre.at(i) + current.at(i) + 1) / 2);
                } else {
                    pos.push_back(pre.at(i));
                    i++;
                    while (i != pre.size() && pre.at(i) == INT16_MAX) {
                        pos.push_back(pre.at(i));
                        i++;
                    }
                    if (i != pre.size()){
                        pos.push_back(pre.at(i) + 1);
                    } else {
                        this->_symbols.at(index - 1).getPos().push_back(1);
                        pos.push_back(50);
                    }
                }
            }
        }
    } else if (index == 0){                             // symbol has to be inserted in the first position
        if (this->_symbols.empty()) {                    // first symbol
            pos.push_back(1);
        } else {
            current = this->_symbols.at(index).getPos();

            // generate vector for position
            // copy current vector into pos vector as long as elements are equal to 1
            for (int i = 0 ; i < current.size() && !flag ; i++ ){
                if(current.at(i) == 1) {
                    pos.push_back(1);
                } else {
                    pos.push_back(current.at(i) - 1);
                    flag = true;
                }
            }

            // if current is all 1s
            if(!flag){
                this->_symbols.at(index).getPos().push_back(2);
                pos.push_back(1);
            }
        }
    } else if (index == this->_symbols.size()){          // symbol has to be inserted in the last position
        pre = this->_symbols.at(index - 1).getPos();

        // generate vector for position
        // copy current vector into pos vector as long as elements are equal to 1
        for (int i = 0 ; i < pre.size() && !flag ; i++ ){
            if(pre.at(i) == INT16_MAX) {
                pos.push_back(INT16_MAX);
            } else {
                pos.push_back(pre.at(i) + 1);
                flag = true;
            }
        }

        // if current is all INT16_MAX
        if(!flag){
            this->_symbols.at(index - 1).getPos().push_back(1);
            pos.push_back(2);
        }
    } else {
        std::cerr << "Wrong index";
        return;
    }

    // insert new symbol
    Symbol s{value, this->_siteId, this->_counter++, pos};
    this->_symbols.insert(this->_symbols.begin() + index, s);

    // prepare message
    Message m{true, this->_siteId, this->_counter - 1, value, pos};
    this->_server.send(m);
}

void SharedEditor::localErase(int index) {
    Symbol s = this->_symbols.at(index);

    // prepare message
    Message m{false, s.getIdClient(), s.getNum()};
    this->_server.send(m);

    // erase symbol
    this->_symbols.erase(this->_symbols.begin() + index);
}

void SharedEditor::process(const Message &m) {
    bool action = m.isInsert();
    bool flag = true;
    const std::vector<int> &pos = m.getPos();

    if (action){                // insert
        for(int i = 0; i < this->_symbols.size() && flag ; i++){
            for(int j = 0; j < pos.size() ; j++){
                if (j == this->_symbols.at(i).getPos().size()) {
                    // insertion
                    Symbol s{m.getValue(), m.getSiteId(), m.getNum(), pos};
                    this->_symbols.insert(this->_symbols.begin() + i + 1, s);
                    flag = false;
                    break;
                } else if (pos.at(j) > this->_symbols.at(i).getPos().at(j))
                    break;
                else if (pos.at(j) < this->_symbols.at(i).getPos().at(j)){
                    // insertion
                    Symbol s{m.getValue(), m.getSiteId(), m.getNum(), pos};
                    this->_symbols.insert(this->_symbols.begin() + i, s);
                    flag = false;
                    break;
                }
            }
        }

        // symbol has to be inserted last
        if(flag){
            Symbol s{m.getValue(), m.getSiteId(), m.getNum(), pos};
            this->_symbols.insert(this->_symbols.end(), s);
        }
    } else {                    // delete
        for(int i = 0; i < this->_symbols.size() ; i++){
            auto &_symbol = this->_symbols.at(i);
            if (m.getSiteId() == _symbol.getIdClient() && m.getNum() == _symbol.getNum()){
                this->_symbols.erase(this->_symbols.begin() + i);
            }
        }
    }
}

std::string SharedEditor::to_string() {
    std::string text;

    for (auto &_symbol : this->_symbols)
        text += _symbol.getValue();

    return text;
}

int SharedEditor::getSiteId() const {
    return _siteId;
}