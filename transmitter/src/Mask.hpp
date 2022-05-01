// Chris McVickar
#pragma once

#include "Image.hpp"
#include "Matrix.hpp"
#include "colors.hpp"
#include "common.hpp"

#include <tuple>
#include <vector>

namespace transmitter {
/// This will include fonts and symbols that can be colored or used to mask
/// images (smiley faces, shapes, etc.)
class Mask : public Matrix<double> {
  public:
    /// Constructs from image
    Mask(const Image &);
    /// Constructs from size
    Mask(int numrows, int numcols);

    /// Inverts mask
    Mask invert() const;
    /// Returns masked image
    Image maskImage(Image) const;
    /// Returns the mask as an image with optional foreground and background
    /// colors
    Image toImage(const common::Color &fg_color = colors::White,
                  const common::Color &bg_color = colors::Blank);

  protected:
    Mask(const Matrix<double> &);
};
} // namespace transmitter