////////////////////////////////////////////////////////////////
/// 2015-2022 University of Idaho
///
/// It would be much better if we had a dedicated changelog if we want to keep
/// the below notes.
///
/// @author Benjamin Jeffery
/// compile: gcc -O2 ht13.c -lftdi -lncurses -o ht13
/// this program transmits dmx-style rgb packets
/// modified to adjust to software PWM on the Mrfs
/// broadcasts from PC/Xbee to Mrf/Arduino
///
/// really going to try constant broadcast this time
/// and... it flickers really badly on holds
///
/// @author Lucas Jackson
/// millisec() is copied from unicon/src/common/time.c
/// for testing purposes. Thanks Clint
///
/// Some minor edits/cleanup
///
/// @author @haydenccarroll Hayden Carroll
/// Some more minor edits by Hayden Carroll
///
/// @author @lotkey Chris McVickar
////////////////////////////////////////////////////////////////
#include "FTDI.hpp"
#include "colors.hpp"
#include "common.hpp"

#include <ftdi.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>

#define DOT 100000
#define DASH 300000
#define SLOW 500000
#define DORK 150000
#define DROOL 1000000
#define DAB 50000
#define SLP 40000
constexpr int ImageWidth = 16;
constexpr int ImageHeight = 16;
constexpr int Vendor = 0x0403;
constexpr int Product = 0x6001;
constexpr int Baudrate = 57600;

void flashColor(transmitter::FTDI& ftdi, common::Color const& color);

int main()
{
  transmitter::FTDI ftdi = transmitter::FTDI(Vendor, Product, Baudrate);
  char letter;    // current letter user entered
  int nbytes = 0; // number of bytes a package sent (global variable)

  std::cerr << "Broadcasting." << std::endl;

  // open curses session for display purposes
  std::cout
    << "Hello, welcome to Ben's Halftime Toolkit!\n"
    << "a=twinkle routine; r=red flash; e=green flash; b=blue flash\n"
    << "d=dark; g=gold flash;  w=white flash; t=test\n"
    << "n=gold on; o=white on; v=orange\n"
    << "m=magenta; y=yellow flash;  k=cyan flash\n"
    << "c=christmas sparkle;  f=twinkle9; h=twinkle9; j=twinkle10; l=twinkle11\n"
    << "s=rainbow short;  p=rainbow med\n"
    << "q=sparkle; x=XMas solid; z=slow twinkle\n"
    << "[=marqee one way; ]=marqee the other;\n"
    << "(=slow marqee one way; )=slow marqee the other;\n\n"
    << " comma key <,> to stop loop\n"
    << " dot key <.> to quit\n";

  system("stty raw"); // enter raw mode so no newline is required for getchar

  // loop until a '.' character is detected
  while (true) {
    letter = getchar();
    switch (letter) {
      case 'r': {
        flashColor(ftdi, transmitter::colors::Red);
        break;
      }
      case 'b': {
        flashColor(ftdi, transmitter::colors::Blue);
        break;
      }
      case 'e': {
        flashColor(ftdi, transmitter::colors::Green);
        break;
      }
      case 'd': {
        ftdi.write(transmitter::Image(ImageHeight, ImageWidth));
        break;
      }
      case 'g': {
        flashColor(ftdi, transmitter::colors::Gold);
        break;
      }
      case 'w': {
        flashColor(ftdi, transmitter::colors::White);
        break;
      }
      case 'n': {
        ftdi.write(transmitter::Image(
          ImageHeight, ImageWidth, transmitter::colors::Gold));
        break;
      }
      case 'o': {
        ftdi.write(transmitter::Image(
          ImageHeight, ImageWidth, transmitter::colors::White));
        break;
      }
      case 'v': {
        ftdi.write(transmitter::Image(
          ImageHeight, ImageWidth, transmitter::colors::Orange));
        break;
      }
      case 'm': {
        ftdi.write(transmitter::Image(
          ImageHeight, ImageWidth, transmitter::colors::Magenta));
        break;
      }
      case 'y': {
        flashColor(ftdi, transmitter::colors::Yellow);
        break;
      }
      case 'k': {
        flashColor(ftdi, transmitter::colors::Cyan);
        break;
      }
      case '.': {
        ftdi.close(); // unnecessary, will close on destruction
        std::cerr << "End of program" << std::endl;
        return EXIT_SUCCESS;
      }
      default: {
        usleep(DAB);
        break;
      }
    }
  }
}

/// This is a temporary utility function. This functionality will be moved over
/// to FTDI (or some other place) once the threading situation is figured out.
void flashColor(transmitter::FTDI& ftdi, common::Color const& color)
{
  ftdi.write(transmitter::Image(ImageHeight, ImageWidth, color));
  usleep(SLP);
  ftdi.write(transmitter::Image(ImageHeight, ImageWidth));
}