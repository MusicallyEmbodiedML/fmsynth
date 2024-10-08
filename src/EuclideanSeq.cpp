#include "EuclideanSeq.hpp"
#include <cstdio>
#include <cmath>
extern "C" {
#include "xassert.h"
}


EuclideanSeq::EuclideanSeq() :
    is_init_(0),
    params_{ 0 }
{
}


bool EuclideanSeq::Process(float phasor)
{
    bool result = false;

    if (is_init_) {

        phasor += offset_;
        while (phasor >= 1){
            phasor -= 1;
        }
        

        // NOTE: Phasor is the last arg
        const float fi         = phasor * params_.n;
        int i                  = static_cast<int>(fi);
        const float rem        = fi - i;
        if (i == params_.n)
        {
            i--;
        }
        const int idx = ((i + params_.n) * params_.k) % params_.n;
        result        = static_cast<bool>(idx < params_.k && rem < pulse_width_ ? 1 : 0);
    
    }

    return result;
}


void EuclideanSeq::SetParams(params p)
{
    // n
    if (p.n <= 0) {
        std::printf("Eucl- n is < 1, rounding to 1.\n");
        p.n = 1;
    }
    // k
    if (p.k >= p.n) {
        std::printf("Eucl- k is > p, rounding to %d.\n", p.n);
        p.k = p.n;
    } else if (p.k <= 0) {
        std::printf("Eucl- k is < 1, rounding to 1.\n");
        p.k = 1;
    }
    // offset
    if (p.offset_d <= 0) {
        std::printf("Eucl- offset_d is < 1, rounding to 1.\n");
        p.offset_d = 1;
    }
    // k
    if (p.offset_n >= p.offset_d) {
        std::printf("Eucl- offset_n is > offset_d, wrapping around.\n");
        while (p.offset_n >= p.offset_d) {
            p.offset_n -= p.offset_d;
        }
    }
    if (p.offset_n < 0) {
        std::printf("Eucl- offset_n is < 0, rounding to 0.\n");
        p.offset_n = 0;
    }

    params_ = p;
    offset_ = static_cast<float>(p.offset_n) /
              static_cast<float>(p.offset_d);
}


void EuclideanSeq::MapNNParams(std::vector<float> nn_params)
{
    constexpr float k_n_min = 1;
    constexpr float k_n_max = 32;
    constexpr float k_n_range = k_n_max - k_n_min;

    // n
    float n = nn_params[0] * k_n_range;
    n += k_n_min;
    n = std::round(n);

    // k
    float k_range = n - k_n_min;
    float k = nn_params[1] * k_range;
    k += k_n_min;
    k = std::round(k);

    // offset
    float offset_d = nn_params[3] * k_n_range;
    offset_d += k_n_min;
    offset_d = std::round(offset_d);
    float offset_n_range = offset_d - k_n_min;
    float offset_n = nn_params[2] * offset_n_range;


    const params set_p {
        static_cast<int>(n),
        static_cast<int>(k),
        static_cast<int>(offset_n),
        static_cast<int>(offset_d),
    };

    SetParams(set_p);
}
