#include "../../AudioFile/AudioFile.h"
#include <string>
#include <vector>
#include <chrono>
#include <tgmath.h>

#include "Compressor.hpp"
#include "Delay.hpp"
#include "Common.hpp"

int main(){
    afx_common::CommonParams common_params(128, 2, 44100.0f);
    AudioFile<float> input_stream;
    std::string output_file_name = "/home/hfords/projs/audiofx/test_files/eg002";
    input_stream.load(output_file_name + ".wav");

    const int number_of_buffers = input_stream.getNumSamplesPerChannel() / common_params.buffer_length();
    AudioFile<float> output_stream;
    AudioFile<float>::AudioBuffer output_buffer;
    output_buffer.resize(common_params.number_of_channels());
    output_buffer[0].resize(number_of_buffers * common_params.buffer_length());
    output_buffer[1].resize(number_of_buffers * common_params.buffer_length());
    std::vector<float> buffer(common_params.buffer_size());

    for(auto it = buffer.begin(); it != buffer.end(); it++){
        std::cout << *it << std::endl;
    }

    Compressor compressor(common_params);
    Delay delay(common_params);
    
    for(size_t b = 0; b < number_of_buffers; b++){
        for(size_t s = 0; s < common_params.buffer_length(); s++){
            for(size_t c = 0; c < common_params.number_of_channels(); c++){
                buffer[common_params.number_of_channels() * s + c] = input_stream.samples[c][s + b * common_params.buffer_length()];
            }
        }

        compressor.compress(&buffer[0]);
        delay.delay(&buffer[0]);
        
        for(size_t s = 0; s < common_params.buffer_length(); s++){
            for(size_t c = 0; c < common_params.number_of_channels(); c++){
                output_buffer[c][s + b * common_params.buffer_length()] = buffer[common_params.number_of_channels() * s + c];
            }
        }
    }
    
    output_stream.setAudioBuffer(output_buffer);
    output_stream.setNumSamplesPerChannel(number_of_buffers * common_params.buffer_length());
    output_stream.setNumChannels(common_params.number_of_channels());
    output_stream.setBitDepth(16);
    output_stream.setSampleRate(44100);
    output_stream.save(output_file_name + "_cppout.wav");
    
    return 0;
}