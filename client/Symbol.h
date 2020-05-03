//
// Created by antonio on 25/04/20.
//

#ifndef EDITOR_SYMBOL_H
#define EDITOR_SYMBOL_H
#include <iostream>
#include <vector>


class Symbol {
private:
    char character;
    std::vector<int> position;
    std::string identifier;
public:
    Symbol(char character, std::string identifier, std::vector<int> position);

    char getCharacter() const;

    void setCharacter(char character);

    const std::vector<int> &getPosition() const;

    void setPosition(const std::vector<int> &position);
};


#endif //EDITOR_SYMBOL_H
