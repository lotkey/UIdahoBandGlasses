#pragma once

#include "colors.hpp"
#include "common/common.hpp"

#include <tuple>
#include <vector>

namespace transmitter {
/// This will include fonts and symbols that can be colored or used to mask
/// images (smiley faces, shapes, etc.)
class Mask : public common::Matrix<double> {
  public:
    Mask(const common::Image &);
    Mask(int numrows, int numcols);

    Mask invert() const;
    common::Image maskImage(common::Image) const;
    common::Image toImage(const common::Color &fg_color = colors::White,
                          const common::Color &bg_color = colors::Blank);

  protected:
    Mask(const Matrix<double> &);
};
} // namespace transmitter