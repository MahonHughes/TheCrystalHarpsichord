//
//  main.cpp
//  The Crystal Harpsichord
//
//  Mahon Hughes
//

#include "pal/pal.h"
#include "modelgenerator.hpp"
#include "string.hpp"
#include "stringinharmonicity.hpp"
#include <iostream>

using namespace pal;

int main(int argc, char **argv)
{
    std::cout << "PROGRAM BEGIN\n";
    std::cout.flush();
    
    // Declares instance of audio interface
    RealTimeAudio audio;
    
    // Declares instance of The Crystal Harpsichord Model
    ModelGenerator modGen;
    
    // Initialise audio parameters
    audio.callback = [&](int numSamples, int numChannels, float *in, float *out)
    {
        for (int sample = 0; sample < numSamples; sample++)
        {
            audio.sampleRate = 44100;
            
            // Define audio source as modGen
            float y = modGen.getNext();
            for (int channel = 0; channel < numChannels; channel++)
            {
                // Write it to the output array here.
                out[sample * numChannels + channel] = y;
            }
        }
    };

    // Create GUI
    Gui gui(800, 600, "The Crystal Harpsichord");

    while (gui.draw())
    {
        ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Audio Setup");
            audio.draw();
        ImGui::End();
        
        ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("Genarated Sound");
            modGen.draw();
        ImGui::End();
        
        ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("String Properties");
            modGen.string.draw();
        ImGui::End();
        
        ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_FirstUseEver);
        ImGui::Begin("String Inharmonicity Properties");
        modGen.string.inharmonicity.draw();
        ImGui::End();
    }

    return 0;
}
