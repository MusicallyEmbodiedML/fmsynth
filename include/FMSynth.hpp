#ifndef _FM_HPP
#define _FM_HPP

#include <cstdint>

#include "maximilian.h"

#include <vector>
#include <array>


const size_t kN_synthparams = 14;
using synthparams_array = std::array<float, kN_synthparams>;


class FMOperator {
public:
    void UpdateParams(void) {
        carrier.UpdateParams();
        modulator.UpdateParams();
    }
    float play(MAXITYPE carrierFreq, MAXITYPE modFreq, MAXITYPE index) {
        float mod = modulator.sinebuf(modFreq);
        float car = carrier.sinebuf(carrierFreq + (mod * index)) ;
        return car;
    }
private:
    maxiOsc carrier, modulator;
};




class FMSynth {
 public:
    static void GenParams(std::vector<float> &param_vector);
    FMSynth(float sample_rate);
    float process();
    int32_t processInt();
    void mapParameters(std::vector<float> &params);

 private:
   FMOperator op1, op2, op3, op4;
   synthparams_array synthparams;
   // FMOperator fmops[10];

};

#endif  // _FM_HPP