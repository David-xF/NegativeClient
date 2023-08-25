#pragma once

#include <code/code.h>

namespace mc {
    int toInt(float input) {
        bool negative = input < 0.0f;
    	if (negative) input *= -1.0f; 

    	uint32_t output = static_cast<uint32_t>(input);
    	if (negative) {
    		output -= 0x80000000;
            output  = 0x80000000 - output;
    		output -= 1;
    	}
    
    	return output;
    }

    float toFloat(int input) {
        bool negative = (input & 0x80000000) == 0x80000000;
        unsigned int output_tmp = input;
        if (negative) {
            output_tmp -= 0x80000000;
            output_tmp  = 0x80000000 - output_tmp;
        }

        float output = static_cast<float>(output_tmp);
        if (negative) {
            uint32_t output_hex = (uint32_t) ((&output)[0]);
            output_hex = output_hex + 0x80000000;
            output = *(float *) &output_hex;
        }

        return output;
    }
}