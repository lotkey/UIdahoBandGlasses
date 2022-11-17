/// @brief Flashes red, green, and blue for half a second before exiting
#include "FTDI.hpp"
#include "Instruction.hpp"
#include "Writer.hpp"
#include "colors.hpp"
#include "config.hpp"

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

  writer.write(transmitter::Flash(transmitter::colors::Red, 0.5));
  writer.queue(transmitter::Flash(transmitter::colors::Green, 0.5));
  writer.queue(transmitter::Flash(transmitter::colors::Blue, 0.5));
  writer.finish();
}