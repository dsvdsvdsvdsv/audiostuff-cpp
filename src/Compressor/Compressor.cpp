#ifndef compressor_cpp
#define compressor_cpp

#include "Compressor.hpp"

Compressor::Compressor(afx_common::CommonParams common_params){
    CompressorParams compressor_params;
    set_common_params(common_params);
    set_compressor_params(compressor_params);
}

Compressor::Compressor(afx_common::CommonParams common_params, CompressorParams compressor_params){
    set_common_params(common_params);
    set_compressor_params(compressor_params);
}

void Compressor::set_compressor_params(CompressorParams compressor_params){
    constexpr float very_small = 0.1f; // used to prevent division by zero
    compressor_params_ = compressor_params;
    
    if(compressor_params.ratio < very_small) compressor_params.ratio = very_small;
    if(compressor_params.knee_width_db < very_small) compressor_params.knee_width_db = very_small;
    if(compressor_params.attack_time_ms < very_small) compressor_params.attack_time_ms = very_small;
    if(compressor_params.release_time_ms < very_small) compressor_params.release_time_ms = very_small;
    
    gain_computer_param_[0] = (1 / compressor_params.ratio - 1) / (2 * compressor_params.knee_width_db);
    gain_computer_param_[1] = (compressor_params.knee_width_db / 2) - compressor_params.threshold_db;
    gain_computer_param_[2] = compressor_params.threshold_db * (1 - 1 / (compressor_params.ratio));
    gain_computer_param_[3] = 1 / (compressor_params.ratio);
    
    attack_filter_coeff_[0] = afx_common::exp(-1 / (1e-3 * compressor_params.attack_time_ms * (float) common_params_.sampling_rate()));
    attack_filter_coeff_[1] = 1 - attack_filter_coeff_[0];
        
    release_filter_coeff_[0] = afx_common::exp(-1 / (1e-3 * compressor_params.release_time_ms * (float) common_params_.sampling_rate()));
    release_filter_coeff_[1] = 1 - release_filter_coeff_[0];
    
    pre_gain_linear_ = db_to_linear(compressor_params.pre_gain_db);
    post_gain_linear_ = db_to_linear(compressor_params.post_gain_db);

    sustain_time_samples_ = 1e-3 * compressor_params_.sustain_time_ms * common_params_.sampling_rate();
}

void Compressor::set_common_params(afx_common::CommonParams common_params){
    common_params_ = common_params;
}

void Compressor::compress(float* audio_in){
    for(int i = 0; i < common_params_.buffer_length(); i++){
	    float sum_of_audio_channels = 0.0f;
        for(int j = 0; j < common_params_.number_of_channels(); j++){
            audio_in[common_params_.number_of_channels() * i + j] *= pre_gain_linear_;
            sum_of_audio_channels += audio_in[common_params_.number_of_channels() * i + j] / common_params_.number_of_channels();
        }
        
        float audio_level_db = linear_to_db(sum_of_audio_channels);
        float gain_computer_out_db = gain_computer(audio_level_db);
        float level_detector_out_db = level_detector(audio_level_db - gain_computer_out_db);
        float final_gain_linear = db_to_linear(-level_detector_out_db);

        for(int j = 0; j < common_params_.number_of_channels(); j++){
            audio_in[2 * i + j] *= post_gain_linear_ * final_gain_linear;
        }
        count++;
    }
}

float Compressor::gain_computer(float input){
    const float test = 2 * (input - compressor_params_.threshold_db);
    float output = 0.0f;
    if(test < -compressor_params_.knee_width_db){
        output = input;
    }
    else if(afx_common::abs(test) <= compressor_params_.knee_width_db){
        output = input + (gain_computer_param_[0] * afx_common::pow(input + gain_computer_param_[1], 2));
    }
    else if(test > compressor_params_.knee_width_db){
        output = gain_computer_param_[2] + (input * gain_computer_param_[3]);
    }
    return output;
}

float Compressor::level_detector(float input){
    const float input_squared = input * input;
    float output_squared = 0.0f;
    if(input_squared > level_detector_previous_out_){
        output_squared = (attack_filter_coeff_[0] * level_detector_previous_out_) + (attack_filter_coeff_[1] * input_squared);
        sustain_count_samples_ = 0;
    }
    else if(sustain_count_samples_ < sustain_time_samples_){
        output_squared = level_detector_previous_out_;
    }
    else{
        output_squared = (release_filter_coeff_[0] * level_detector_previous_out_) + (release_filter_coeff_[1] * input_squared);
    }
    sustain_count_samples_++;
    level_detector_previous_out_ = output_squared;
    return afx_common::sqrt(output_squared);
}

float Compressor::linear_to_db(float linear){
    const float linear_abs = afx_common::abs(linear);
    float output = 0.0f;
    if(linear_abs > 0.0f){
        output = 20.0f * afx_common::log10(linear_abs);
    }
    else{
        output = -200.0f;
    }
    return output;
}

float Compressor::db_to_linear(float db){
    return afx_common::pow(10.0f, db / 20.0f);
}
#endif