/// @brief Transmits green until the user hits the Enter key
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
  std::string tmp;

  writer.write(transmitter::On(transmitter::colors::Green));
  std::getline(std::cin, tmp); // wait until the user hits 'Enter'
  writer.quit();
}