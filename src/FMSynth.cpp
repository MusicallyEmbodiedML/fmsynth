#include "FMSynth.hpp"
#include <cmath>
#include <random>
#include <cstdlib>
#include <vector>

//extern "C" {
//    #include <stdio.h>
//}

// #ifndef M_PI
//     #define M_PI 3.14159265358979323846
// #endif

void FMSynth::GenParams(std::vector<float> &param_vector)
{
#if 0
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> dis(0.f, 1.0f);
#else
    float rand_scale = 1.f / static_cast<float>(RAND_MAX);
#endif
    //printf("Calling FMSynth::GenParams\n");

    for(size_t i=0; i < kN_synthparams; i++) {
        param_vector[i] = std::rand() * rand_scale;
        //printf(".");
    }
    //printf("\n");
}

FMSynth::FMSynth(float sample_rate) :
    smoother_(100.f, sample_rate)
{
    std::srand(0);
    // w_ = 2.f * M_PI * freq_ / sample_rate_;
    // phase_ = 0;
    
    maxiSettings::setup(sample_rate, 1, 16);
    std::vector<FMOperator> ops { op1, op2, op3, op4 };
    for (auto &op : ops) {
        op.UpdateParams();
    }

    std::vector<float> randParams(kN_synthparams);
    GenParams(randParams);

    mapParameters(randParams);
}

void FMSynth::mapParameters(std::vector<float> &params) {
    float *params_ptr = params.data();
    float *dest_ptr = synthparams.data();
    
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = (*(params_ptr++) * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ =  (params[9] * 100);

    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ = 20 + ((*(params_ptr) * *(params_ptr)) * 5000);
    ++params_ptr;
    *dest_ptr++ =  (*(params_ptr++) * 100);

    *dest_ptr++ = (*(params_ptr++) * 100);


}

float FMSynth::process()
{
    // Smooth all parameters before using them
    smoother_.Process(synthparams.data(), synthparams_smoothed.data());

#if 1
    float w = op1.play(synthparams_smoothed[0] + 
        (op2.play(synthparams_smoothed[3],synthparams_smoothed[4],synthparams_smoothed[5]) * synthparams_smoothed[6]),
        synthparams_smoothed[1], synthparams_smoothed[2]);
#endif
#if 1
    float w2 = op3.play(synthparams_smoothed[7] + 
        (op4.play(synthparams_smoothed[10],synthparams_smoothed[11],synthparams_smoothed[12]) * synthparams_smoothed[13]),
        synthparams_smoothed[8], synthparams_smoothed[9]);
    // float w2 = op3.play(220 + (op4.play(231,111,5) * 40),20,50);
    return w + w2;
#endif
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
    static const float scaling = std::pow(2.f, 31.f) - 1000.f;

    return static_cast<int32_t>(process() * scaling);
}
