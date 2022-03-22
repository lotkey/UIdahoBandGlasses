#pragma once

#include "common/common.hpp"
#include "transmitter/src/colors.hpp"

#include <vector>

namespace transmitter {
/// This will include fonts and symbols that can be colored or used to mask
/// images (smiley faces, shapes, etc.)
class Symbol {
  public:
    Symbol(int numrows, int numcols);

    Symbol invert() const;
    common::Image mask(const common::Image &) const;
    common::Image toImage(const common::Color &fg_color = colors::White,
                          const common::Color &bg_color = colors::Blank) const;

  private:
    int m_numrows;
    int m_numcols;
    std::vector<std::vector<bool>> m_data;
};
} // namespace transmitter