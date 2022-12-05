/// @brief Alternates between lime and magenta for every newline
/// Quit by entering 'q'
#include "FTDI.hpp"
#include "Instruction.hpp"
#include "Writer.hpp"
#include "colors.hpp"
#include "config.hpp"

#include <iostream>
#include <string>

constexpr int ImageHeight = 16;
constexpr int ImageWidth = 16;
constexpr int Vendor = 0x0403;
constexpr int Product = 0x6001;
constexpr int Baudrate = 57600;

int main()
{
  transmitter::config::setImageHeight(ImageHeight);
  transmitter::config::setImageWidth(ImageWidth);
  transmitter::FTDI ftdi(Vendor, Product, Baudrate);
  transmitter::Writer writer(ftdi);
  bool lime = true;
  std::string input;

  std::cerr << "Type 'q' to quit." << std::endl;
  while (true) {
    if (lime) {
      writer.write(transmitter::On(transmitter::colors::Lime));
    } else {
      writer.write(transmitter::On(transmitter::colors::Magenta));
    }
    std::getline(std::cin, input);
    if (input == "q") { break; }
    lime = !lime;
  }

  writer.quit();
}