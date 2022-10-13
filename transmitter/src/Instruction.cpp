#include "Instruction.hpp"
#include "Config.hpp"

namespace transmitter {
std::list<std::pair<Image, double>> const& Instruction::writes() const
{
  return m_writes;
}

On::On(Image const& img) { m_writes.push_back({img, -1.0}); }

On::On(common::Color const& color)
{
  m_writes.push_back(
    {Image(config::imageHeight(), config::imageWidth(), color), -1.0});
}

Off::Off()
{
  m_writes.push_back({Image(config::imageHeight(), config::imageWidth()), 0.1});
}

Flash::Flash(Image const& img, double durationSeconds)
{
  m_writes.push_back({img, durationSeconds});
  m_writes.push_back(
    {Image(config::imageHeight(), config::imageWidth()), 0.25});
}

Flash::Flash(common::Color const& color, double durationSeconds)
{
  m_writes.push_back({Image(config::imageHeight(), config::imageWidth(), color),
                      durationSeconds});
  m_writes.push_back({Image(config::imageHeight(), config::imageWidth()), 0.1});
}
} // namespace transmitter