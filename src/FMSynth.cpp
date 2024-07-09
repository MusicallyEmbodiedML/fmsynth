#include "FMSynth.hpp"
#include <cmath>
#include <random>
#include <vector>


// #ifndef M_PI
//     #define M_PI 3.14159265358979323846
// #endif


FMSynth::FMSynth(float sample_rate)
{
    // w_ = 2.f * M_PI * freq_ / sample_rate_;
    // phase_ = 0;
    
    maxiSettings::setup(sample_rate, 1, 16);  

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> dis(0.f, 1.0f);

    std::vector<float> randParams(12);
    for(size_t i=0; i < synthparams.size(); i++) {
        randParams[i] = dis(gen);
    }  
    mapParameters(randParams);
}

void FMSynth::mapParameters(std::vector<float> &params) {
    synthparams[0] = 20 + (powf(params[0],2) * 5000);
    synthparams[1] = 20 + (powf(params[1],2) * 5000);
    synthparams[2] =  (params[2] * 100);

    synthparams[3] = 20 + (powf(params[3],2) * 5000);
    synthparams[4] = 20 + (powf(params[4],2) * 5000);
    synthparams[5] =  (params[5] * 100);

    synthparams[6] =  (params[6] * 100);

    synthparams[7] = 20 + (powf(params[7],2) * 5000);
    synthparams[8] = 20 + (powf(params[8],2) * 5000);
    synthparams[9] =  (params[9] * 100);

    synthparams[10] = 20 + (powf(params[10],2) * 5000);
    synthparams[11] = 20 + (powf(params[11],2) * 5000);
    synthparams[12] =  (params[12] * 100);

    synthparams[13] =  (params[13] * 100);


}

float FMSynth::process()
{
    float w = op1.play(synthparams[0] + 
        (op2.play(synthparams[3],synthparams[4],synthparams[5]) * synthparams[6]),
        synthparams[1], synthparams[2]);
    float w2 = op3.play(synthparams[7] + 
        (op4.play(synthparams[10],synthparams[11],synthparams[12]) * synthparams[13]),
        synthparams[8], synthparams[9]);
    // float w2 = op3.play(220 + (op4.play(231,111,5) * 40),20,50);
    return w + w2;
    // float w=0;
    // for(size_t i=0; i < 10; i++) {
    //     w += fmops[i].play(200 + i, 200+i, 2 + i);
    // }
    // return w/10.0;
    // float y = std::sin(phase_);
    // phase_ += w_;
    // if (phase_ > 2 * M_PI) {
    //     phase_ -= 2 * M_PI;
    // }
    // return y;
}

int32_t FMSynth::processInt()
{
    const float scaling = std::pow(2.f, 31.f) - 1000.f;

    return static_cast<int32_t>(process() * scaling);
}
