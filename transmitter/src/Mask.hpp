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
    /// Constructs from image
    Mask(const common::Image &);
    /// Constructs from size
    Mask(int numrows, int numcols);

    /// Inverts mask
    Mask invert() const;
    /// Returns masked image
    common::Image maskImage(common::Image) const;
    /// Returns the mask as an image with optional foreground and background
    /// colors
    common::Image toImage(const common::Color &fg_color = colors::White,
                          const common::Color &bg_color = colors::Blank);

  protected:
    Mask(const Matrix<double> &);
};
} // namespace transmitter