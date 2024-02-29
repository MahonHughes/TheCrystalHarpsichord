//
//  modelgenerator.hpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 16/04/2022.
//

#ifndef modelgenerator_hpp
#define modelgenerator_hpp

#include <stdio.h>
#include "string.hpp"
#include <iostream>
#include <vector>
#include "body.hpp"

class ModelGenerator{
public:
    void draw();
    float getNext();
    String string;
private:
    float sampleRate = 44100;
};

#endif /* modelgenerator_hpp */
