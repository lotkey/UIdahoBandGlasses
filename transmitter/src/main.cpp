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
#include "Instruction.hpp"
#include "Writer.hpp"
#include "colors.hpp"
#include "common.hpp"
#include "config.hpp"

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

int main()
{
  transmitter::config::setImageHeight(ImageHeight);
  transmitter::config::setImageWidth(ImageWidth);
  transmitter::FTDI ftdi(Vendor, Product, Baudrate);
  transmitter::Writer writer(ftdi);
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
    << " dot key <.> to quit\n";

  system("stty raw"); // enter raw mode so no newline is required for getchar

  // loop until a '.' character is detected
  while (true) {
    letter = getchar();
    switch (letter) {
      case 'r': {
        writer.write(transmitter::Flash(transmitter::colors::Red, 0.5));
        break;
      }
      case 'b': {
        writer.write(transmitter::Flash(transmitter::colors::Blue, 0.5));
        break;
      }
      case 'e': {
        writer.write(transmitter::Flash(transmitter::colors::Green, 0.5));
        break;
      }
      case 'd': {
        writer.write(transmitter::On(transmitter::colors::Blank));
        break;
      }
      case 'g': {
        writer.write(transmitter::Flash(transmitter::colors::Gold, 0.5));
        break;
      }
      case 'w': {
        writer.write(transmitter::Flash(transmitter::colors::White, 0.5));
        break;
      }
      case 'n': {
        writer.write(transmitter::On(transmitter::colors::Gold));
        break;
      }
      case 'o': {
        writer.write(transmitter::On(transmitter::colors::White));
        break;
      }
      case 'v': {
        writer.write(transmitter::On(transmitter::colors::Orange));
        break;
      }
      case 'm': {
        writer.write(transmitter::On(transmitter::colors::Magenta));
        break;
      }
      case 'y': {
        writer.write(transmitter::Flash(transmitter::colors::Yellow, 0.5));
        break;
      }
      case 'k': {
        writer.write(transmitter::Flash(transmitter::colors::Cyan, 0.5));
        break;
      }
      case '.': {
        writer.quit();
        std::cerr << "\nEnd of program." << std::endl;
        return EXIT_SUCCESS;
      }
      default: {
        usleep(DAB);
        break;
      }
    }
  }
}