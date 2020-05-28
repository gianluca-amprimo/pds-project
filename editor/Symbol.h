//
// Created by antonio on 25/04/20.
//

#ifndef EDITOR_SYMBOL_H
#define EDITOR_SYMBOL_H
#include <iostream>
#include <vector>


class Symbol {
private:
    wchar_t character;
    std::vector<int> position;
    std::wstring identifier;
public:
    Symbol(wchar_t character, std::wstring identifier, std::vector<int> position);

    wchar_t getCharacter() const;

    void setCharacter(wchar_t character);

    const std::vector<int> &getPosition() const;

    void setPosition(const std::vector<int> &position);
};


#endif //EDITOR_SYMBOL_H
