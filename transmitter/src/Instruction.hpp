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
  std::list<std::pair<Image, double>> m_writes;

  virtual void abstracter() = 0;
};

class On : public Instruction {
public:
  virtual ~On() = default;

  On(Image const& img);
  On(common::Color const& color);

private:
  virtual void abstracter() override {}
};

class Off : public Instruction {
public:
  virtual ~Off() = default;

  Off();

private:
  virtual void abstracter() override {}
};

class Flash : public Instruction {
public:
  virtual ~Flash() = default;

  Flash(Image const& img, double durationSeconds);
  Flash(common::Color const&, double durationSeconds);

private:
  virtual void abstracter() override {}
};
} // namespace transmitter