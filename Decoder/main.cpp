#include <iostream>
#include <string>
#include <vector>
#include "adaptive_model.h"
#include "arithmetic_encoder.h"
#include "arithmetic_decoder.h"

int main() 
{
  std::cout << "\nEnter Message: ";
  std::string message;
  std::getline(std::cin, message);

  const int EOF_SYMBOL = 256;
  std::vector<int> data;
  for (char ch : message) 
  {
    data.push_back(static_cast<int>(ch));
  }
  data.push_back(EOF_SYMBOL);

  // Encoding
  AdaptiveModel modelEncoder;
  ArithmeticEncoder encoder;
  for (int symbol : data) 
  {
    encoder.encodeSymbol(symbol, modelEncoder);
    modelEncoder.update(symbol);
  }
  std::string encoded_bits = encoder.finish();
  std::cout << "\nEncoded bit stream:\n" << encoded_bits << "\n";

  double entropy = static_cast<double>(encoded_bits.length()) / static_cast<double>(data.size() - 1);  
  std::cout << "Entropy of the encoded message: " << entropy << " bits per symbol\n";

  // Decoding
  AdaptiveModel modelDecoder;
  ArithmeticDecoder decoder(encoded_bits);
  std::vector<int> decodedSymbols;

  while (true) 
  {
    int symbol = decoder.decodeSymbol(modelDecoder);
    modelDecoder.update(symbol);

    if (symbol == EOF_SYMBOL)
    {
      break;
    }
    decodedSymbols.push_back(symbol);
  }

  std::string decodedMessage;

  for (int s : decodedSymbols) 
  {
    decodedMessage.push_back(static_cast<char>(s));
  }

  std::cout << "\nDecoded message:\n" << decodedMessage << "\n";


  return 0;
}
