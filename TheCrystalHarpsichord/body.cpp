//
//  body.cpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 19/04/2022.
//

#include "body.hpp"
#include <fstream>
#include <iostream>
#include <vector>

Body::Body(){
    // Open file
    std::ifstream file;
    file.open("body.txt");
    
    // Define IR vector
    std::vector<float> sounds;
    float number;
    
    // Read from file
    if (file.is_open()){
        while (file >> number) {
            sounds.push_back(number);
        }
        file.close();
    }
    // Set IR
    IR = sounds;
}
