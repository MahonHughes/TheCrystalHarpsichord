//
//  stringinharmonicity.cpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 20/04/2022.
//

#include "stringinharmonicity.hpp"

float StringInharmonicity::getInharmonicity(){
    //Calculate inharmonicity parameter
    return (powf(M_PI,3) * E * powf(radius,4)) / (16 * powf(length,2) * tention);
}

// Draw GUI
void StringInharmonicity::draw(){
    ImGui::SliderFloat("Length of string (m)", &length, 0.1, 2, "%.3f", 4);
    ImGui::SliderFloat("Radius of string (m)", &radius, 0.0001, 0.01, "%.4f", 4);
    ImGui::SliderFloat("Young's modulus", &E, 1*powf(10,11), 10*powf(10,13), "%.1f", 4);
    ImGui::SliderFloat("Tention (N)", &tention, 1, 200, "%.6f", 6);

    // Define user-selectable presets for Standard and Diamond string
    if (ImGui::Button("Standard String")){
        length = 2;
        radius = 0.0009;
        E = 2.1*powf(10,11);
        tention = 81;
    }
    if (ImGui::Button("Diamond String")){
        length = 2;
        radius = 0.0009;
        E = 5.5*powf(10,12);
        tention = 81;
    }
}
