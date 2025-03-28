#include "arithmetic_encoder.h"
#include <iostream>

ArithmeticEncoder::ArithmeticEncoder() : low(0), high(TOP_VALUE), pending_bits(0), current_byte(0), bit_count(0) {}

void ArithmeticEncoder::outputBit(int bit)
{
  current_byte = (current_byte << 1) | bit;
  bit_count++;

  if (bit_count == 8)
  {
    output.push_back(current_byte);
    current_byte = 0;
    bit_count = 0;
  }

  for (int i = 0; i < pending_bits; i++)
  {
    if (bit_count == 8)
    {
      output.push_back(current_byte);
      current_byte = 0;
      bit_count = 0;
    }
  }
  pending_bits = 0;
}



void ArithmeticEncoder::encodeSymbol(int symbol, AdaptiveModel& model)
{
  std::cout << "Encoding symbol: " << symbol << "\n";
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

void ArithmeticEncoder::scale() 
{
  low = low * 2;
  high = high * 2 + 1;
}

std::vector<unsigned char> ArithmeticEncoder::finish()
{
  outputBit(low >= FIRST_QUARTER ? 1 : 0);
  for (int i = 0; i < pending_bits; i++)
  {
    outputBit(low >= FIRST_QUARTER ? 0 : 1); 
  }
  for (int i = CODE_VALUE_BITS - 2; i >= 0; --i)
  {
    outputBit((low >> i) & 1);
  }

  if (bit_count > 0)
  {
    output.push_back(current_byte << (8 - bit_count));
  }
  std::cout << "Encoder finished, bitstream size: " << output.size() << " bytes\n";
  return output;
}
