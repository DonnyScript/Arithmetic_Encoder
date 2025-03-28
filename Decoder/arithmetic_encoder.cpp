#include "arithmetic_encoder.h"

ArithmeticEncoder::ArithmeticEncoder() : low(0), high(TOP_VALUE), pending_bits(0) {}

void ArithmeticEncoder::encodeSymbol(int symbol, AdaptiveModel& model) 
{
  auto range = model.getRange(symbol);
  unsigned int cum_low = range.first;
  unsigned int cum_high = range.second;
  unsigned int total = model.total;
  unsigned int range_width = high - low + 1;
  high = low + (range_width * cum_high) / total - 1;
  low = low + (range_width * cum_low) / total;
  unsigned int underflow_counter = 0;

  unsigned int high_for_calc = (high << 16) | 0xFFFF;
  unsigned int low_for_calc = (low << 16);

  unsigned int first_bit_mask = 0x80000000;
  unsigned int second_bit_mask = 0x40000000;
  unsigned int clear_mask = 0x7FFFFFFF;
  while (true) 
  {
    if((high_for_calc & first_bit_mask) == (low_for_calc & first_bit_mask)) //This means that the MSB of low and high are the same, which means we can output a bit.
    {
      outputBit((low_for_calc & first_bit_mask) ? 1 : 0); // Output the bit corresponding to the MSB of low (or high, since they are the same)
      low_for_calc = (clear_mask & low_for_calc) << 1;      // shifts low and replaces with a 0
      high_for_calc = ((clear_mask & high_for_calc) << 1) | 1; // shifts high and replaces with a 1
    }
    else if((high_for_calc & second_bit_mask) == (low_for_calc & second_bit_mask))
    {
      // Splice out the second MSB from `high` and `low`
      // Remove the second MSB from `low`
      low_for_calc = ((low_for_calc & ~second_bit_mask) << 1) | (low_for_calc & 0x80000000 ? 0x80000000 : 0);
      // Remove the second MSB from `high`
      high_for_calc = (((high_for_calc & ~second_bit_mask) << 1) | 1) | (high_for_calc & 0x80000000 ? 0x80000000 : 0);
      pending_bits++;
    }
    else{
      break;
    }
  }
  low = static_cast<unsigned short>(low_for_calc >> 16);
  high = static_cast<unsigned short>(high_for_calc >> 16);
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
