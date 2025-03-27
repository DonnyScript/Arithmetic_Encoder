#ifndef ARITHMETIC_ENCODER_H
#define ARITHMETIC_ENCODER_H

#include <string>
#include "adaptive_model.h"

const int CODE_VALUE_BITS = 16;
const int TOP_VALUE = (1 << CODE_VALUE_BITS) - 1;         // 65535
const int FIRST_QUARTER = (TOP_VALUE + 1) / 4;              // 16384
const int HALF = 2 * FIRST_QUARTER;                         // 32768
const int THIRD_QUARTER = 3 * FIRST_QUARTER;                // 49152

class ArithmeticEncoder 
{
  public:
    ArithmeticEncoder();
    void encodeSymbol(int symbol, AdaptiveModel& model);
    std::string finish();
    std::string output; // Contains the encoded bit stream

  private:
    int low, high;
    int pending_bits;
    void outputBit(int bit);
    void scale();
};

#endif // ARITHMETIC_ENCODER_H
