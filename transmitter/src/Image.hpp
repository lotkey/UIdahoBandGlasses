// Chris McVickar
#pragma once

#include "Color.hpp"
#include "Matrix.hpp"

#include <png.h>

#include <functional>
#include <vector>

namespace common {
class Image : public Matrix<Color> {
  public:
    /// @returns A random image with the specified size
    static Image noise(int numrows, int numcols);

    /// Construct an image from a given size and an optional fill color
    Image(int numrows, int numcols, const Color &fillColor = {});
    /// Construct an image from a 2D vector array
    Image(const std::vector<std::vector<Color>> &data);
    Image resize(int numRows, int numCols);

    /// @returns The image with every pixel inverted
    Image invert() const;
    /// @returns The image recolored with the provided color
    Image recolor(const Color &) const;
    /// @returns The image with no saturation
    Image grayscale() const;
    /// @param s The saturation [0.0-1.0]
    /// @returns The image with the provided saturation
    Image saturate(double s) const;
    /// @param h The hue [0.0-1.0]
    /// @returns The image with the provided hue
    Image hue(double h) const;
    /// @returns The image with only black and white pixels
    Image blackAndWhite() const;
    /// @returns A 2D vector of encoded colors
    std::vector<uint8_t> encoded() const;

  protected:
    Image(const Matrix<Color> &);

    /// Applies the function to all pixels
    void apply(const std::function<void(Color &)> &);
};
} // namespace common