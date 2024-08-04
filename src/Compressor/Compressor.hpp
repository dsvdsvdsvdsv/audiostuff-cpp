#ifndef compressor_hpp
#define compressor_hpp

#include <iostream>
#include <string>

#include "Common.hpp"

class Compressor{
  public:
    struct CompressorParams{
      float threshold_db = 0.0f;
      float ratio = 1.0f;
      float knee_width_db = 0.0f;
      float pre_gain_db = 0.0f;
      float post_gain_db = 0.0f;
      float attack_time_ms = 1.0f;
      float release_time_ms = 200.0f;
      float sustain_time_ms = 20.0f;
  };
    Compressor(afx_common::CommonParams common_params);
    Compressor(afx_common::CommonParams common_params, CompressorParams compressor_params);
    void set_compressor_params(CompressorParams compressor_params);
    void set_common_params(afx_common::CommonParams common_params);
    void compress(float* audio_in);
  private:
    // compressor vars
    afx_common::CommonParams common_params_;
    CompressorParams compressor_params_;
    unsigned long count = 0; // how many times the for loop in compress() has ran

    // gain computer vars
    float pre_gain_linear_ = 1.0f;
    float post_gain_linear_ = 1.0f;
    float gain_computer_param_[4];

    // level detector vars
    float level_detector_previous_out_ = 0.0f;
    float release_filter_coeff_[2];
    float attack_filter_coeff_[2];
    unsigned long sustain_count_samples_ = 0;
    unsigned long sustain_time_samples_ = 0;
    
    // functions
    float gain_computer(float input);
    float level_detector(float input);
    float linear_to_db(float linear);
    float db_to_linear(float db);
};
#endif