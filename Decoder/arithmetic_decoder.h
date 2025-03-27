#ifndef ARITHMETIC_DECODER_H
#define ARITHMETIC_DECODER_H

#include <string>
#include "adaptive_model.h"

class ArithmeticDecoder 
{
  public:
    ArithmeticDecoder(const std::string& input_bits);
    int decodeSymbol(AdaptiveModel& model);

  private:
    std::string bits;
    size_t bit_index;
    int low, high, code;
    int readBit();
};

#endif // ARITHMETIC_DECODER_H
