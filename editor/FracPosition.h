//
// Created by antonio on 31/05/20.
//

#ifndef EDITOR_FRACPOSITION_H
#define EDITOR_FRACPOSITION_H


#include <vector>

class FracPosition {

private:
    std::vector<int> position;

public:
    bool operator==(FracPosition fp);
    FracPosition operator+(FracPosition fp);
    FracPosition operator-(FracPosition fp);
    FracPosition operator/(int dividend);

};


#endif //EDITOR_FRACPOSITION_H
