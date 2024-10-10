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
        int ph_mult;
    };
    static constexpr size_t n_params = sizeof(params) / sizeof(int);
    
    EuclideanSeq();
    EuclideanSeq(const std::vector<int> &n_choices);
    bool Process(float phasor);
    void SetParams(params p);
    void MapNNParams(std::vector<float> nn_params);
    inline float Probe(size_t probe_n) {
        return probes_[probe_n];
    }

 protected:

    const std::vector<int> n_choices_;

    static constexpr float pulse_width_ = 0.5;
    bool is_init_;
    params params_;
    float offset_;
    static constexpr size_t n_probes_ = 2;
    float probes_[n_probes_];

    int MakeN_(float param);
    int MakeMul_(float param);
};


#endif  // __EUCLIDEAN_SEQ_HPP__
