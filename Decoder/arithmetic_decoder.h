#ifndef ARITHMETIC_DECODER_H
#define ARITHMETIC_DECODER_H

#include <vector>
#include "adaptive_model.h"

class ArithmeticDecoder 
{
  public:
    ArithmeticDecoder(const std::vector<unsigned char>& input_bits);
    int decodeSymbol(AdaptiveModel& model);

  private:
    std::vector<unsigned char> input_bytes;
    size_t bit_index;
    int low, high, code;
    int readBit();
};

#endif
