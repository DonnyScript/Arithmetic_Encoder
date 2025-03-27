#ifndef ADAPTIVE_MODEL_H
#define ADAPTIVE_MODEL_H

#include <vector>
#include <utility>

const int SYMBOL_COUNT = 257; // Symbols 0-255 plus an EOF symbol (256)
const int MAX_FREQUENCY = 16383;

class AdaptiveModel 
{
  public:
    AdaptiveModel();
    std::pair<int, int> getRange(int symbol) const;
    void update(int symbol);
    int total;
    std::vector<int> cum_freq; // Exposed for decoder use

  private:
    std::vector<int> frequencies;
    void buildCumulative();
    void scaleFrequencies();
};

#endif
