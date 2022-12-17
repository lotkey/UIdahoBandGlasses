#pragma once

#include "Image.hpp"
#include "common.hpp"

#include <list>
#include <utility> // std::pair

namespace transmitter {
/// Base class for instructions to write to the FTDI device
class Instruction {
public:
  virtual ~Instruction() = default;

  std::list<std::pair<Image, double>> const& writes() const;

protected:
  // Hide the constructor so this class can't be constructed, but inherited
  // types can
  Instruction() = default;

  std::list<std::pair<Image, double>> m_writes;
};

/// Instruction to set the FTDI device to transmit something indefinitely, or at
/// least until the next instruction.
class On : public Instruction {
public:
  virtual ~On() = default;

  On(Image const& img);
  On(common::Color const& color);
};

/// Instruction to write a blank image to the FTDI device
class Off : public Instruction {
public:
  virtual ~Off() = default;

  Off();
};

/// Instruction to write a flash to the FTDI device
/// It will write an On message followed by an Off message
class Flash : public Instruction {
public:
  virtual ~Flash() = default;

  Flash(Image const& img, double durationSeconds);
  Flash(common::Color const&, double durationSeconds);
};
} // namespace transmitter