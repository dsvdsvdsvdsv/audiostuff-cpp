#ifndef CommonParams_hpp
#define CommonParams_hpp

namespace afx_common{
	    struct CommonParams{
        // setters
        CommonParams(){}
        
        CommonParams(long buffer_length, long number_of_channels, float sampling_rate){
            this->buffer_length(buffer_length);
            this->number_of_channels(number_of_channels);
            this->sampling_rate(sampling_rate);
        }
        
        void buffer_length(long buffer_length){ 
            buffer_length_ = buffer_length;
        }
        
        void number_of_channels(long number_of_channels){
            number_of_channels_ = number_of_channels;
        }
        
        void sampling_rate(float sampling_rate){
            sampling_rate_ = sampling_rate;
        }
        
        // getters
        long buffer_length(){
            return buffer_length_;
        }
        
        long number_of_channels(){
            return number_of_channels_;
        }
        
        long sampling_rate(){
            return sampling_rate_;
        }
        
        long buffer_size(){
            return number_of_channels_ * buffer_length_;
        }
        
        // what
        bool is_initialized(){
            if (buffer_length_ == -1) return false;
            else if (number_of_channels_ == -1) return false;
            else if (sampling_rate_ == -1) return false;
            else return true;
        }
      private:
        long buffer_length_ = -1;
        long number_of_channels_ = -1;
        float sampling_rate_ = -1;
    };
}

#endif