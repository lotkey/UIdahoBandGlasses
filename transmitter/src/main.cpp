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
#include <nlohmann/json.hpp>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
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

std::map<std::string, transmitter::Image> readLayouts();

int main()
{
  transmitter::config::setImageHeight(ImageHeight);
  transmitter::config::setImageWidth(ImageWidth);
  transmitter::FTDI ftdi(Vendor, Product, Baudrate);
  transmitter::Writer writer(ftdi);
  auto layouts = readLayouts();
  std::string input;

  std::cerr << "Broadcasting." << std::endl;

  std::cerr << "Hello, welcome to the halftime toolkit!\n"
            << "  r = red flash\n"
            << "  e = green flash\n"
            << "  b = blue flash\n"
            << "  d = dark\n"
            << "  g = gold flash\n"
            << "  w = white flash\n"
            << "  n = gold on\n"
            << "  o = white on\n"
            << "  v = orange\n"
            << "  m = magenta\n"
            << "  y = yellow flash\n"
            << "  k = cyan flash\n"
            << " dot key <.> to quit\n";

  if (layouts.size() > 1) {
    std::cerr
      << "You can also enter one of the following keywords to use a custom layout:"
      << std::endl;
    for (auto const& [name, _] : layouts) {
      std::cerr << "  " << name << std::endl;
    }
  }

  while (true) {
    std::getline(std::cin, input);
    if (input == ".") {
      break;
    } else if (layouts.find(input) != layouts.end()) {
      writer.write(transmitter::On(layouts[input]));
    } else {
      switch (input[0]) {
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
      }
    }
  }

  writer.finish();
}

std::map<std::string, transmitter::Image> readLayouts()
{
  if (!std::filesystem::is_directory("./layouts")) {
    std::cerr << "No custom layouts to load." << std::endl;
    return {};
  }

  std::map<std::string, transmitter::Image> layouts;

  for (auto const& entry : std::filesystem::directory_iterator("./layouts")) {
    std::ifstream infile(entry.path().string());
    nlohmann::json data = nlohmann::json::parse(infile);
    transmitter::Image image(ImageHeight, ImageWidth);

    for (auto const& participant : data["layoutDetails"]) {
      std::string colorStr = participant["color"];
      colorStr = colorStr.substr(1);
      int glassesNumber = participant["glassesNumber"];
      glassesNumber -= 1;

      int indexDim0 = glassesNumber / ImageHeight;
      int indexDim1 = glassesNumber - indexDim0;

      auto channelToInt = [](std::string const& channel) {
        int x;
        std::stringstream ss;
        ss << std::hex << channel;
        ss >> x;
        return x;
      };

      int r = channelToInt(colorStr.substr(0, 2));
      int g = channelToInt(colorStr.substr(2, 2));
      int b = channelToInt(colorStr.substr(4, 2));

      image.set(indexDim0, indexDim1, common::Color(r, g, b));
    }

    layouts[entry.path().filename().string()] = image;
  }

  return layouts;
}