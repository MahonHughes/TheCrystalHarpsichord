//
//  string.cpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 16/04/2022.
//

#include "string.hpp"
#include <iostream>
#include "pal/pal.h"
#include <sndfile.h>

using namespace pal;

void String::exportToWAV(const std::vector<float>& audioData, const char* filename) {
    SF_INFO sfInfo;
    sfInfo.channels = 1;  // Assuming mono audio
    sfInfo.samplerate = 44100;  // Set your desired sample rate
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    SNDFILE* sndfile = sf_open(filename, SFM_WRITE, &sfInfo);

    if (!sndfile) {
        std::cerr << "Error opening the output file: " << sf_strerror(NULL) << std::endl;
        return;
    }

    sf_writef_float(sndfile, audioData.data(), audioData.size());

    sf_close(sndfile);
}

//Funtion returns sign of an equation
//Reference: https://stackoverflow.com/a/4609795
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
};

// Template algorithm to perform convolution
// Reference https://stackoverflow.com/a/24519913
template<typename T>
std::vector<T>
conv(std::vector<T> const &f, std::vector<T> const &g) {
  int const nf = f.size();
  int const ng = g.size();
  int const n  = nf + ng - 1;
  std::vector<T> out(n, T());
  for(auto i(0); i < n; ++i) {
    int const jmn = (i >= ng - 1)? i - (ng - 1) : 0;
    int const jmx = (i <  nf - 1)? i            : nf - 1;
    for(auto j(jmn); j <= jmx; ++j) {
      out[i] += (f[j] * g[i - j]);
    }
  }
  return out;
}

// Get impulse responce
String::String(){
    soundOutputVector = body.IR;
}

String::String(int seconds){
    durationOfSimulation = seconds;
}
// Define GUI
void String::draw(){
    ImGui::SliderFloat("Duration (s).", &durationOfSimulation, 1, 20, "%.3f", 4);
    ImGui::SliderFloat("Center location.", &center, 0.001, 1, "%.3f", 4);
    ImGui::SliderFloat("Width of plectrum.", &width, 0.001, 10, "%.3f", 4);
    ImGui::SliderFloat("Inharmonicity Constant.", &BInCond, 0.00001, 1, "%.6f", 6);
    ImGui::SliderFloat("Initial Displacement.", &u0, 0, 10, "%.3f", 4);
    ImGui::SliderFloat("Initial Velocity.", &v0, 0, 10, "%.3f", 4);
    // ImGui::SliderFloat("Fundamental frequency.", &fundamental, 0, 1000, "%.3f", 4);
    ImGui::InputFloat("Fundamental frequency (Hz).", &fundamental, 0, 1000, "%.3f");
    
    // Define e, the radio button choice.
    static int e = 2;
    ImGui::RadioButton("With Body (takes considerably longer)", &e, 0);
    ImGui::RadioButton("Without Body", &e, 1);
    ImGui::RadioButton("Body Impulse Response only", &e, 2);
    
    // Genarate note button
    if (ImGui::Button("Genarate Note")){
        std::cout << "Calculationg note..." << std::endl;
        if (e == 2){
            // IR only
            soundOutputVector = body.IR;
        }else{
            // Simulate string
            soundOutputVector = getArray();
        }
        // If body is also enabled, convalute the IR
        if (e == 0) {
            std::cout << "Resonating (this may take some time)..." << std::endl;
            soundOutputVector = conv(soundOutputVector, body.IR);
        }
        exportToWAV(soundOutputVector, "outputVector.wav");
        std::cout << "Done!" << std::endl;
        currentCounter = 0;
    }
}



// This function runs the simulation
std::vector<float> String::getArray(){
    
    // Calculate inharmonicity parameter
    BInCond = inharmonicity.getInharmonicity();

    // Calculate parameters
    float timeStep = 1/sampleRate;
    float NF = floor(sampleRate*durationOfSimulation);
    float gamma = 2 * fundamental;
    float K = sqrtf(BInCond) * (gamma/M_PI);
    
    float h = sqrt((
                    powf(gamma,2.0)
                    *powf(timeStep,2)
                    +sqrt(
                          powf(gamma,4)
                          *powf(timeStep,4)
                          +32*powf(K,2)
                          *powf(timeStep,2)
                          )
                    )
                   /2);
    
    float N = floor(1/h);
    h = 1/N;
    float mu = K*timeStep/powf(h,2);
    float lambda = gamma*timeStep/h;
    
    int rp_int[2];
    float rp_frac[2];
    float inv_rp_frac[2];
    
    // Initialise read position matrices
    rp_int[0] = 1 + floor(N * positionsOfReading[0]);
    rp_int[1] = 1 + floor(N * positionsOfReading[1]);
    
    rp_frac[0] = 1 + positionsOfReading[0] / h - rp_int[0];
    rp_frac[1] = 1 + positionsOfReading[1] / h - rp_int[1];
    
    inv_rp_frac[0] = 1 - rp_frac[0];
    inv_rp_frac[1] = 1 - rp_frac[1];
    
    // Calculate loss parameters
    float zeta1 = (- (powf(gamma,2))
                   +sqrt(powf(gamma,4)+4*powf(K,2)*powf((2 * M_PI * loss[0][0]),2)))
    /(2*powf(K,2));
    float zeta2 = (-(powf(gamma,2))
                   +sqrt(powf(gamma,4)+4*powf(K,2)*powf((2*M_PI*loss[1][0]),2)))/(2*powf(K,2));
    float sig0 = 6*log(10)*(-zeta2/loss[0][1]+zeta1/loss[0][1])/(zeta1-zeta2);
    float sig1 = 6*log(10)*(1/loss[0][1]-1/loss[1][1])/(zeta1-zeta2);

    int matrixSize = int(N) - 1;
    
    // Initialise and update matrices
    std::vector<std::vector<float>> I; // Identity matrix
    std::vector<std::vector<float>> C;
    std::vector<std::vector<float>> B;
    
    for (int i = 0; i < matrixSize; i++){
        I.push_back(std::vector<float>());
        C.push_back(std::vector<float>());
        B.push_back(std::vector<float>());
    }
    
    for (int n = 0; n < matrixSize; n++){
        for (int m = 0; m < matrixSize; m++){
            I[m].push_back(0);
            C[m].push_back(0);
            B[m].push_back(0);
        }
    }
    
    
    // Calculate update matrices
    for (int i = 0; i < matrixSize-1; i++) {
        I[i][i] = 1;
        
        C[i][i] = I[i][i] + (-sig1 * timeStep / powf(h,2) - sig0 * timeStep / 2);
        C[i+1][i] = I[i+1][i] + sig1 * timeStep / (2 * powf(h,2));
        C[i][i+1] = I[i][i+1] + sig1 * timeStep / (2 * powf(h,2));
        
        B[i][i] = 2 * I[i][i] + (-2 * powf(lambda,2) - 6 * powf(mu,2));
        B[i+1][i] = 2 * I[i+1][i] + (powf(lambda,2) + 4 * powf(mu,2));
        B[i][i+1] = 2 * I[i][i+1] + (powf(lambda,2) + 4 * powf(mu,2));
        
        if (i < matrixSize - 2) {
            B[i+2][i] = 2 * I[i+1][i] - powf(mu,2);
            B[i][i+2] = 2 * I[i][i+1] - powf(mu,2);
        }
    }
    // Add final items to matrices
    I[matrixSize-1][matrixSize-1] = 1;
    C[matrixSize-1][matrixSize-1] = I[matrixSize-1][matrixSize-1] + (-sig1 * timeStep / powf(h,2) - sig0 * timeStep / 2);
    B[matrixSize-1][matrixSize-1] = I[matrixSize-1][matrixSize-1] + (-2 * powf(lambda,2) - 6 * powf(mu,2));
    

    // Create raised cosine conditions
    
    float uArray[matrixSize];
    float u1Array[matrixSize];
    float u2Array[matrixSize];
    
    float *u = uArray;
    float *u1 = u1Array;
    float *u2 = u2Array;
    
    float xax[int(N)-1];
    float ind[int(N)-1];
    float rc[int(N)-1];
    for (int i = 0; i < matrixSize; i++) {
        xax[i] = (i+1) * h;
        ind[i] = sign( std::fmaxf( -(xax[i]-center-width/2) * (xax[i] - center+width /2) , 0));
        rc[i] = 0.5 * ind[i] * (1+cos(2*M_PI*(xax[i]-center)/width));
    }
    
    // Set raised cosine initial diaplacements

    float out[int(NF)][2];
    
    for (int i = 0; i < matrixSize; i++) {
        
        u2[i] = u0 * rc[i];
        u1[i] = (u0 + timeStep * v0) * rc[i];
    }
    
    // Calculate the following outside array
    // Parameters for B and C
    int r1 = matrixSize;
    int c1 = matrixSize;
    
    // MAIN LOOP
    for (int mainLoopIndex = 3; mainLoopIndex < int(NF); mainLoopIndex++) {
        // Matrix Multiplication:: It is not easy to put this into a function
        // This was tried however it is not an easy or efficient thing to do
        // https://stackoverflow.com/a/34279426
        // The following matrix multiplication code is heavily adapted from
        // https://www.tutorialspoint.com/
        // cplusplus-program-to-multiply-two-matrices-by-passing-matrix-to-function

        // Multiply B by u1
        float Bu1[matrixSize];
        float Cu2[matrixSize];

        // Initialise with 0s
        for(int i=0; i<r1; ++i){
            Bu1[i] = 0;
            Cu2[i] = 0;
        }
        // Multiply matrices
        for (int i=0; i<r1; ++i){
            for (int k=0; k<c1; ++k) {
                Bu1[i] += B[i][k]*u1[k];
                Cu2[i] += C[i][k]*u2[k];
            }
        }

        //Bu1 - Cu2
        for (int i = 0; i < matrixSize; i++) {
            u[i] = Bu1[i] - Cu2[i];
        }

        float chanL = (1 - rp_frac[0]) * u[rp_int[0]] + rp_frac[0] * u[rp_int[0]+1];
        float chanR = (1 - rp_frac[1]) * u[rp_int[1]] + rp_frac[1] * u[rp_int[1]+1];

        out[mainLoopIndex][0] = chanL;
        out[mainLoopIndex][1] = chanR;

        // Swap values along using memory references so contents don't have to be copied
        float *uswap = u2;
        u2 = u1;
        u1 = u;
        u = uswap;
    }
    // Define output array
    float averageChans[int(NF)];
    // Average values from read positions
    for (int i = 0; i < int(NF); i ++) {
        averageChans[i] = (out[i][0] + out[i][1])/2;
    }
    // Convert to vector and return
    std::vector<float> averageChansVector(averageChans, averageChans + int(NF));
    return averageChansVector;
}
