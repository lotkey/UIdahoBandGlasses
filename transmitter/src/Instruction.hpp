#pragma once

#include "Image.hpp"
#include "common.hpp"

#include <list>
#include <utility> // std::pair

namespace transmitter {
class Instruction {
public:
  virtual ~Instruction() = 0;

  std::list<std::pair<Image, double>> const& writes() const;

protected:
  std::list<std::pair<Image, double>> m_writes;
};

// add one for flash and one for plain writes
} // namespace transmitter