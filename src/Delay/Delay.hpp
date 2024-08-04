#ifndef delay_hpp
#define delay_hpp

#include <vector>
#include <iostream>
#include <tuple>
#include "Common.hpp"

class Delay{
  public:
      struct DelayParams{
        float delay_ms = 30.0f;
        float modulation_depth_ms = 4.0f;
        float modulation_frequency = 0.5f;
        float dry_gain_linear = 0.5f;
        float wet_gain_linear = 0.5f;
        float feedback_gain_linear = 0.5f;
    };
    Delay(afx_common::CommonParams common_params);
    Delay(afx_common::CommonParams common_params, DelayParams delay_params);
    void set_common_params(afx_common::CommonParams common_params);
    void set_delay_params(DelayParams delay_params);
    void delay(float* audio_in);
  private:
    // vars
    afx_common::CommonParams common_params_;
    DelayParams delay_params_;
    std::vector<float> delay_buffer_;
    long count_samples_ = 0;
    long count_buffers_ = 0;
    long delay_time_samples_ = 0;
    long delay_buffer_size_ = 0;
    long delay_buffer_length_ = 0;

    // functions
    void add_to_delay_buffer(const float* audio_in, float wet_gain_linear);
    std::tuple<long, float> get_delay_amount();
};

#endif