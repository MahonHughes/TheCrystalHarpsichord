//
//  pianostring.hpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 16/04/2022.
//

#ifndef string_hpp
#define string_hpp

#include <stdio.h>
#include <cmath>
#include <vector>
#include "body.hpp"
#include "stringinharmonicity.hpp"

class String{
public:
    String();
    String(int seconds);
    void draw();
    std::vector<float> getArray();
    float getNext();
    int currentCounter = 0;
    void exportToWAV(const std::vector<float>& audioData, const char* filename);
    std::vector<float> soundOutputVector;
    Body body;
    StringInharmonicity inharmonicity;
private:
    float sampleRate = 44100; // (Hz) usually 44100
    float BInCond; // Inharmonicity parameter
    float fundamental = 220; // String fundamental frequency (Hz)
    float durationOfSimulation = 2; // Duration (seconds)
    float center = 0.02; // Location of plectrum
    float width = 0.1; // Width of plectrum
    float u0 = 1; // Maximum initial displacement (initialised at 1 as that's how a pluck works)
    float v0 = 0; // Maximum initial velocity
    float positionsOfReading[2] = {0.3, 0.7}; //Specifies whhere along the string values will be read from
    float loss[2][2] = {
        {100, 10},
        {1000, 8}
    }; // Loss parameters [freq.(Hz), T60(s), freq.(Hz), T60(s)]
    //Pointers to vectors
    std::vector<float> *u;
    std::vector<float> *u1;
    std::vector<float> *u2;
};
#endif /* string_hpp */
