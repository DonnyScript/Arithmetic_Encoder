#include <iostream>
#include <string>
#include <vector>
#include "adaptive_model.h"
#include "arithmetic_encoder.h"
#include "arithmetic_decoder.h"

int main()
{
  // Get user input
  std::cout << "Enter Message: ";
  std::string message;
  std::getline(std::cin, message);

  // Prepare data with EOF symbol
  const int EOF_SYMBOL = 256;
  std::vector<int> data;
  for (char ch : message)
  {
    data.push_back(static_cast<int>(static_cast<unsigned char>(ch)));
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
  std::vector<unsigned char> encoded_bits = encoder.finish();

  // Print encoded bits
  std::cout << "Encoded bits: ";
  for (unsigned char byte : encoded_bits)
  {
    for (int bit = 7; bit >= 0; --bit)
    {
      std::cout << ((byte >> bit) & 1);
    }
  }
  std::cout << std::endl;

  // Calculate and print average bits per symbol
  double bits_per_symbol = (encoded_bits.size() * 8.0) / data.size();
  std::cout << "Average bits per symbol: " << bits_per_symbol << "\n";

  // Decoding
  AdaptiveModel modelDecoder;
  ArithmeticDecoder decoder(encoded_bits);
  std::vector<int> decodedSymbols;
  const int MAX_SYMBOLS = 10000; // Safety limit
  int symbol_count = 0;

  while (symbol_count < MAX_SYMBOLS)
  {
    int symbol = decoder.decodeSymbol(modelDecoder);
    modelDecoder.update(symbol);
    if (symbol == EOF_SYMBOL)
    {
      break;
    }
    decodedSymbols.push_back(symbol);
    symbol_count++;
  }

  if (symbol_count == MAX_SYMBOLS)
  {
    std::cerr << "Decoder exceeded maximum symbols without finding EOF.\n";
    return 1;
  }

  // Reconstruct decoded message
  std::string decodedMessage;
  for (int s : decodedSymbols)
  {
    decodedMessage.push_back(static_cast<char>(s));
  }

  std::cout << "\nDecoded message:\n" << decodedMessage << "\n";

  return 0;
}