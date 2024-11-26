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
    smoother_(100.f, sample_rate),
    midi_enabled_(true)
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

inline float midiNoteToFrequency(int midiNote) {
    // Constants
    constexpr float A440 = 440.0f; // Frequency of A4
    constexpr float SEMITONE_RATIO = 1.059463094359f; // 2^(1/12), the ratio between adjacent semitones

    // Middle C (C4) is MIDI note 60, A4 is MIDI note 69
    int semitoneOffset = midiNote - 69;

    // Calculate the frequency
    return A440 * std::pow(SEMITONE_RATIO, semitoneOffset);
}

float FMSynth::process()
{
    static float synth_freq = -1.f;
    // Smooth all parameters before using them
    smoother_.Process(synthparams.data(), synthparams_smoothed.data());

    float carrier_1, carrier_2, envelope;
#if 0
    // Handle MIDI
    if (midi_enabled_) {
        ts_midi_note *last_note_ptr = note_buffer_.back();
        if (last_note_ptr == nullptr) {
            // No notes to play
            carrier_1 = synthparams_smoothed[0];
            carrier_2 = synthparams_smoothed[7];
            envelope = 0;
        } else {
            // One note to play!
            carrier_1 = midiNoteToFrequency(last_note_ptr->note_number);
            carrier_2 = carrier_1;
            envelope = last_note_ptr->velocity;

            if (carrier_1 != synth_freq) {
                std::printf("Freq: %f\n", carrier_1);
                synth_freq = carrier_1;
            }
        }
    } else {
        carrier_1 = synthparams_smoothed[0];
        carrier_2 = synthparams_smoothed[7];
        envelope = 1.0f;
    }
#endif

#if 0
    float w = op1.play(carrier_1 + 
        (op2.play(synthparams_smoothed[3],synthparams_smoothed[4],synthparams_smoothed[5]) * synthparams_smoothed[6]),
        synthparams_smoothed[1], synthparams_smoothed[2]);
#endif
#if 0
    float w2 = op3.play(carrier_2 + 
        (op4.play(synthparams_smoothed[10],synthparams_smoothed[11],synthparams_smoothed[12]) * synthparams_smoothed[13]),
        synthparams_smoothed[8], synthparams_smoothed[9]);
    // float w2 = op3.play(220 + (op4.play(231,111,5) * 40),20,50);
    //return (w + w2) * envelope;
#endif

    carrier_1 = 440.f;
    envelope = 0.2;
    static float phase_ = 0;
    float w_ = carrier_1 / 48000.f;
    float y = std::sin(phase_);
    phase_ += w_;
    while (phase_ > 1) {
        phase_ -= 1;
    }
    return y * envelope;
}

int32_t FMSynth::processInt()
{
    static const float scaling = std::pow(2.f, 31.f) - 1000.f;

    return static_cast<int32_t>(process() * scaling);
}

void FMSynth::EnableMIDI(bool en)
{
    midi_enabled_ = en;
}

void FMSynth::AddMIDINote(ts_midi_note note)
{
    if (midi_enabled_) {
        if (note.velocity > 0) {
            note_buffer_.push_back(note);
            std::printf("Velocity: %f\n", note.velocity);
        } else {
        #if 0
            note_buffer_.RemoveNote(note);
        #else
            note_buffer_.clear();
        #endif
        }
    }
}