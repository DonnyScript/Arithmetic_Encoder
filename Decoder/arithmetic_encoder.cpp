#include "arithmetic_encoder.h"

ArithmeticEncoder::ArithmeticEncoder() : low(0), high(TOP_VALUE), pending_bits(0) {}

void ArithmeticEncoder::encodeSymbol(int symbol, AdaptiveModel& model) 
{
  auto range = model.getRange(symbol);
  int cum_low = range.first;
  int cum_high = range.second;
  int total = model.total;
  int range_width = high - low + 1;
  high = low + (range_width * cum_high) / total - 1;
  low = low + (range_width * cum_low) / total;

  while (true) 
  {
    if (high < HALF) 
    {
      outputBit(0);
      scale();
    }
    else if (low >= HALF) 
    {
      outputBit(1);
      low -= HALF;
      high -= HALF;
      scale();
    }
    else if (low >= FIRST_QUARTER && high < THIRD_QUARTER) 
    {
      pending_bits++;
      low -= FIRST_QUARTER;
      high -= FIRST_QUARTER;
      scale();
    }
    else 
    {
      break;
    }
  }
}

void ArithmeticEncoder::outputBit(int bit) 
{
  output.push_back(bit ? '1' : '0');

  for (int i = 0; i < pending_bits; i++) 
  {
    output.push_back(bit ? '0' : '1');
  }

  pending_bits = 0;
}

void ArithmeticEncoder::scale() 
{
  low = low * 2;
  high = high * 2 + 1;
}

std::string ArithmeticEncoder::finish() 
{
  pending_bits++;
  if (low < FIRST_QUARTER) 
  {
    outputBit(0);
  }
  else 
  {
    outputBit(1);
  }

  return output;
}
