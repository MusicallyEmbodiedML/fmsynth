#pragma once

//#define NDEBUG
#include <cassert>
#include <vector>

const size_t kN_synthparams = 14;

template<size_t N>
class maxtrixMixer {
public:
    matrixMixer() {
        //clear matrix
        (for auto &v: mixingMatrix) {
            v = 0.f;
        }
    }

    randomise_linear(float low, float high) {
        float randMaxInv = 1.f / RAND_MAX;
        for(auto &v: mixingMatrix) {
            v = ((rand() * randMaxInv) * (high-low)) + low;
        }
    }

    set(std::vector &newValues) {
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
            float mixValue = inputs[i] * mixingMatrix[N+i];
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