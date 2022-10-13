#pragma once

#include "Image.hpp"
#include "common.hpp"

#include <list>
#include <utility> // std::pair

namespace transmitter {
class Instruction {
public:
  virtual ~Instruction() = default;

  std::list<std::pair<Image, double>> const& writes() const;

protected:
  // Hide the constructor so it can only be called by inherited types
  Instruction() = default;

  std::list<std::pair<Image, double>> m_writes;
};

class On : public Instruction {
public:
  virtual ~On() = default;

  On(Image const& img);
  On(common::Color const& color);
};

class Off : public Instruction {
public:
  virtual ~Off() = default;

  Off();
};

class Flash : public Instruction {
public:
  virtual ~Flash() = default;

  Flash(Image const& img, double durationSeconds);
  Flash(common::Color const&, double durationSeconds);
};
} // namespace transmitter