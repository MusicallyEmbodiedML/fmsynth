#ifndef _FM_HPP
#define _FM_HPP

#include <cstdint>

#include "maximilian.h"

#include <vector>
#include <array>
#include <cmath>


const size_t kN_synthparams = 14;
using synthparams_array = std::array<float, kN_synthparams>;


template<size_t n_channels>
class OnePoleSmoother {
 public:
    OnePoleSmoother(float time_ms, float sample_rate) :
        sample_rate_(sample_rate),
        y_ { 0 } {
        SetTimeMs(time_ms);
    }
    void SetTimeMs(float time_ms) {
        //b1_ = std::exp(
        //    std::log(0.01) /
        //    time_ms * sample_rate_ * 0.001
        //);
        b1_ = std::pow(0.1, 1.f/ (time_ms * 0.001 * sample_rate_));
    }
    __attribute__((always_inline)) void Process(const float * x_ptr, float *y_ptr) {
        float *y2_ptr = y_;
        for (unsigned int c = 0; c < n_channels; c++) {
            const float x = *x_ptr; 
            *y2_ptr = *y_ptr = x + b1_ * (*y2_ptr - x);
            ++x_ptr;
            ++y_ptr;
            ++y2_ptr;
        }
    }

 protected:
    float sample_rate_;
    float b1_;
    float y_[n_channels];
};


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
   synthparams_array synthparams_smoothed;
   // FMOperator fmops[10];
   OnePoleSmoother<kN_synthparams> smoother_;

};

#endif  // _FM_HPP