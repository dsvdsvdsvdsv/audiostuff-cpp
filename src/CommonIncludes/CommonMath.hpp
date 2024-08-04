#ifndef CommonMath_hpp
#define CommonMath_hpp

#include <cmath>

namespace afx_common{
	inline float pow(float base, float exp){
		return std::pow(base, exp);
	}

	inline float exp(float num){
		return std::exp(num);
	}

	inline float abs(float num){
		return std::abs(num);
	}

	inline float sqrt(float num){
		return std::sqrt(num);
	}

	inline float log10(float num){
		return std::log10(num);
	}

	inline float sin(float num){
		return std::sin(num);
	}

	inline float floor(float num){
		return std::floor(num);
	}

}

#endif