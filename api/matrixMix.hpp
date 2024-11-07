#pragma once

//#define NDEBUG
#include <cassert>
#include <vector>

const size_t kN_synthparams = 14;

template<size_t N>
class MaxtrixMixApp {
public:

    MaxtrixMixApp() {
    }

    float play(float x) {
        float flange = flanger.flange(x, params[3] * 6000 + 100, params[4] * 0.98, params[5] * 10.f, params[6]) * params[1];
        float dist = distortion.fastAtanDist(x, params[7] * 10.0) * params[2] * 0.5;
        float delayed = dl.play(x, params[8] * 10000 + 100, params[9] * 0.99) * params[0];
        x = (flange + dist + delayed);
        return x;
    }

    void mapParameters(std::vector<float> &newparams) {
        for(size_t i=0; i < params.size(); i++) {
            params[i] = newparams[i];
        }
    }

private:
    maxiNonlinearity distortion;
    maxiDelayline<50000> dl;
    maxiOsc osc;
    maxiFlanger<15000> flanger;

    std::vector<float> params{kN_synthparams,0.5};
};

template<size_t N>
class matrixMixer {
public:
    matrixMixer() {
        //clear matrix
        for (auto &v: mixingMatrix) {
            v = 0.f;
        }
    }

    void randomise_linear(float low, float high) {
        float randMaxInv = 1.f / RAND_MAX;
        for(auto &v: mixingMatrix) {
            v = ((rand() * randMaxInv) * (high-low)) + low;
        }
    }

    void set(std::vector<float> &newValues) {
        assert(mixingMatrix.size() == newValues.size());
        for(size_t i=0; i < mixingMatrix.size(); i++) {
            mixingMatrix[i] = newValues[i];
        }
    }

    float calculateMix(const std::array<float,N> &inputs, const size_t outputIndex) {
        assert(inputs.size() == N);
        float sum=0;
        size_t offset = N * outputIndex;
        for(size_t i=0; i < N; i++) {
            float mixValue = inputs[i] * mixingMatrix[offset+i];
            if (i == outputIndex) {
                mixValue *= directFeedbackScale;
            }
            sum += mixValue;

        }
        return sum;
    }

    void setDirectFeedbackScaling(const float scale) {
        directFeedbackScale = scale;
    }

    void scaleWithEigenValues(float alpha) {
        //is this needed?
    }

private:
    std::array<float, N*N> mixingMatrix;
    float directFeedbackScale = 0.f;
    
};