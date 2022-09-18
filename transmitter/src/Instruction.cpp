#include "Instruction.hpp"

namespace transmitter {
std::list<std::pair<Image, double>> const& Instruction::writes() const
{
  return m_writes;
}
} // namespace transmitter