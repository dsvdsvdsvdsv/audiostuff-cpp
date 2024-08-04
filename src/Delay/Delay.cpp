#ifndef delay_cpp
#define delay_cpp

#include "Delay.hpp"

Delay::Delay(afx_common::CommonParams common_params){
    DelayParams delay_params;
    set_common_params(common_params);
    set_delay_params(delay_params);
}

Delay::Delay(afx_common::CommonParams common_params, DelayParams delay_params){
    set_common_params(common_params);
    set_delay_params(delay_params);
}

void Delay::set_common_params(afx_common::CommonParams common_params){
    common_params_ = common_params;
}

void Delay::set_delay_params(DelayParams delay_params){
    delay_params_ = delay_params;
    delay_time_samples_ = common_params_.sampling_rate() * (delay_params.delay_ms + delay_params.modulation_depth_ms) / 1000.0f + 1;
    delay_buffer_length_ = common_params_.buffer_length() + delay_time_samples_;
    delay_buffer_size_ = common_params_.number_of_channels() * delay_buffer_length_;
    delay_buffer_.resize(delay_buffer_size_);
}

void Delay::delay(float* audio_in){
    const long delay_buffer_pos = (common_params_.buffer_length() * count_buffers_) * common_params_.number_of_channels();
    for(auto i = 0; i < common_params_.buffer_length(); i++){
        for(auto j = 0; j < common_params_.number_of_channels(); j++){
            std::tuple<long, float> a = get_delay_amount();

            const long x = i * common_params_.number_of_channels() + j;
            const long delay_pos = (delay_buffer_pos + x + std::get<0>(a)) % delay_buffer_size_;
            const long delay_pos_plus1 = (delay_buffer_pos + x + std::get<0>(a) + 1) % delay_buffer_size_;
            const long feedback_pos = (delay_buffer_pos + x) % delay_buffer_size_;

            // add to delay_buffer
            delay_buffer_[delay_pos] += (1 - std::get<1>(a)) * audio_in[x];
            delay_buffer_[delay_pos_plus1] += std::get<1>(a) * audio_in[x];
            
            // get from delay_buffer
            audio_in[x] = (audio_in[x] * delay_params_.dry_gain_linear) + (delay_params_.wet_gain_linear * delay_buffer_[feedback_pos]);
            
            // add feedback to delay_buffer
            delay_buffer_[delay_pos] += (1 - std::get<1>(a)) * delay_params_.feedback_gain_linear * delay_buffer_[feedback_pos];
            delay_buffer_[delay_pos_plus1] += std::get<1>(a) * delay_params_.feedback_gain_linear * delay_buffer_[feedback_pos];
            delay_buffer_[feedback_pos] = 0;
        }
        count_samples_++;
    }
    count_buffers_++;
}

std::tuple<long, float> Delay::get_delay_amount(){
    float time_s = (float) count_samples_ / (float) common_params_.sampling_rate();
    float sine_part = afx_common::sin(2.0f * (float) M_PI * delay_params_.modulation_frequency * (time_s));
    float delay_s = (delay_params_.delay_ms / 1000.0f) + (delay_params_.modulation_depth_ms / 2.0f / 1000.0f) * (1.0f + powf(sine_part, 3));
    float delay_amount_samples = delay_s * common_params_.sampling_rate();
    float fraction = delay_amount_samples - afx_common::floor(delay_amount_samples);
    return std::tuple<long, float>{delay_amount_samples, fraction};
}
#endif