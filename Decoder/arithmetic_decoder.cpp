#include "arithmetic_decoder.h"
#include "arithmetic_encoder.h"  // For constants like TOP_VALUE, HALF, etc.

ArithmeticDecoder::ArithmeticDecoder(const std::string& input_bits)
  : bits(input_bits), bit_index(0), low(0), high(TOP_VALUE), code(0) 
{
  for (int i = 0; i < CODE_VALUE_BITS; i++) 
  {
    code = (code << 1) | readBit();
  }
}

int ArithmeticDecoder::readBit() 
{
  if (bit_index < bits.size()) 
  {
    return bits[bit_index++] - '0';
  }

  return 0;
}

int ArithmeticDecoder::decodeSymbol(AdaptiveModel& model) 
{
  int total = model.total;
  int range_width = high - low + 1;
  int value = ((code - low + 1) * total - 1) / range_width;

  int symbol = 0;
  while (model.cum_freq[symbol + 1] <= value)
  {
    symbol++;
  }

  auto range = model.getRange(symbol);
  int cum_low = range.first;
  int cum_high = range.second;
  high = low + (range_width * cum_high) / total - 1;
  low = low + (range_width * cum_low) / total;

  while (true) 
  {
    if (high < HALF) 
    {
      // do nothing
    }
    else if (low >= HALF) 
    {
      code -= HALF;
      low -= HALF;
      high -= HALF;
    }
    else if (low >= FIRST_QUARTER && high < THIRD_QUARTER) 
    {
      code -= FIRST_QUARTER;
      low -= FIRST_QUARTER;
      high -= FIRST_QUARTER;
    }
    else 
    {
      break;
    }

    low = low * 2;
    high = high * 2 + 1;
    code = (code << 1) | readBit();
  }

  return symbol;
}
