#include "adaptive_model.h"

AdaptiveModel::AdaptiveModel() 
{
  frequencies.assign(SYMBOL_COUNT, 1);
  cum_freq.assign(SYMBOL_COUNT + 1, 0);
  buildCumulative();
}

void AdaptiveModel::buildCumulative() 
{
  cum_freq[0] = 0;
  for (int i = 0; i < SYMBOL_COUNT; i++) 
  {
    cum_freq[i + 1] = cum_freq[i] + frequencies[i];
  }
  total = cum_freq[SYMBOL_COUNT];
}

void AdaptiveModel::scaleFrequencies() 
{
  for (int i = 0; i < SYMBOL_COUNT; i++) 
{
    frequencies[i] = (frequencies[i] + 1) / 2;
    if (frequencies[i] < 1)
    { 
      frequencies[i] = 1;
    }
  }
}

std::pair<int, int> AdaptiveModel::getRange(int symbol) const 
{
  return { cum_freq[symbol], cum_freq[symbol + 1] };
}

void AdaptiveModel::update(int symbol) 
{
  frequencies[symbol] += 1;
  if (total >= MAX_FREQUENCY)
  {
    scaleFrequencies();
  }
  buildCumulative();
}
