//
// Created by antonio on 31/05/20.
//

#include "FracPosition.h"

bool FracPosition::operator==(FracPosition fp) {
    bool equals = true;


    // if size of positions is different, no check required
    if(this->position.size() != fp.position.size()) {
        equals = false;
    }else{
        for(int i = 0; i < this->position.size(); i++){
            if(this->position[i] != fp.position[i]) equals = false;
        }
    }
    return equals;
}

FracPosition FracPosition::operator+(FracPosition fp) {
    return FracPosition();
}

FracPosition FracPosition::operator-(FracPosition fp) {
    return FracPosition();
}

FracPosition FracPosition::operator/(int dividend) {
    return FracPosition();
}
