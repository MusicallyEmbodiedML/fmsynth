#ifndef _FM_HPP
#define _FM_HPP

#include <cstdint>

#include "maximilian.h"

#include <vector>


class FMOperator {
public:
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
    FMSynth(float sample_rate);
    float process();
    int32_t processInt();
    void mapParameters(std::vector<float> &params);

 private:
   FMOperator op1, op2, op3, op4;
   std::vector<float> synthparams = std::vector<float>(14);
   // FMOperator fmops[10];

};

#endif  // _FM_HPP