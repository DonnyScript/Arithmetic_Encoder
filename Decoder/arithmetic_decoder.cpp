#include "arithmetic_decoder.h"
#include "arithmetic_encoder.h"  // For constants like TOP_VALUE, HALF, etc.
#include <iostream>

ArithmeticDecoder::ArithmeticDecoder(const std::vector<unsigned char>& input_bits)
  : input_bytes(input_bits), bit_index(0), low(0), high(TOP_VALUE), code(0) 
{
  for (int i = 0; i < CODE_VALUE_BITS; i++) 
  {
    code = (code << 1) | readBit();
  }
}

int ArithmeticDecoder::readBit() 
{
  if (bit_index >= input_bytes.size() * 8)
  {
    return 0;
  }

  int byte_index = bit_index / 8;
  int bit_offset = bit_index % 8;
  bit_index++;
  return (input_bytes[byte_index] >> (7 - bit_offset)) & 1;
}

int ArithmeticDecoder::decodeSymbol(AdaptiveModel& model) {
  int total = model.total;
  int range_width = high - low + 1;
  int value = ((code - low + 1) * total - 1) / range_width;

  auto it = std::upper_bound(model.cum_freq.begin(), model.cum_freq.end(), value);
  int symbol = it - model.cum_freq.begin() - 1;

  auto range = model.getRange(symbol);
  int cum_low = range.first;
  int cum_high = range.second;
  high = low + (range_width * cum_high) / total - 1;
  low = low + (range_width * cum_low) / total;

  std::cout << "Decoded symbol: " << symbol << ", low: " << low << ", high: " << high << ", code: " << code << "\n";

  while (true) {
    if (high < HALF) {
    }
    else if (low >= HALF) {
      code -= HALF;
      low -= HALF;
      high -= HALF;
    }
    else if (low >= FIRST_QUARTER && high < THIRD_QUARTER) {
      code -= FIRST_QUARTER;
      low -= FIRST_QUARTER;
      high -= FIRST_QUARTER;
    }
    else {
      break;
    }
    low = low * 2;
    high = high * 2 + 1;
    code = (code << 1) | readBit();
  }
  return symbol;
}
