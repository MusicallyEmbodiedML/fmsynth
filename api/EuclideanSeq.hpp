#ifndef __EUCLIDEAN_SEQ_HPP__
#define __EUCLIDEAN_SEQ_HPP__

#include <vector>

class EuclideanSeq {

 public:

    struct params {
        int n;
        int k;
        int offset_n;
        int offset_d;
    };
    
    EuclideanSeq();
    bool Process(float phasor);
    void SetParams(params p);
    void MapNNParams(std::vector<float> nn_params);

 protected:

    static constexpr float pulse_width_ = 0.5;
    bool is_init_;
    params params_;
    float offset_;

};


#endif  // __EUCLIDEAN_SEQ_HPP__
