//
//  modelgenerator.cpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 16/04/2022.
//

#include "modelgenerator.hpp"
#include "pal/pal.h"
#include <iostream>

using namespace pal;

// Draw window section
void ModelGenerator::draw(){
    // Access vector like array by getting pointer to first element
    float* vectorStartLocation = &string.soundOutputVector[0];
    // Plot waveform
    ImGui::PlotLines("Waveform", vectorStartLocation, string.soundOutputVector.size());
    // Define button to replay note
    if (ImGui::Button("Play Note Again")){
        string.currentCounter = 0;
    }
}

// Output next value in waveform
float ModelGenerator::getNext()
{
    string.currentCounter++;
    // If sample is over, return 0
    if (string.currentCounter > string.soundOutputVector.size()) {
        return 0;
    }else{
        float y = string.soundOutputVector[string.currentCounter];
        return y;
    }
}
