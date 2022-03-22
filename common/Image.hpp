#pragma once

#include "Color.hpp"

#include <tuple>
#include <vector>

namespace common {
class Image {
  public:
    /// Constructs an image from dimensions, with an optional fill color
    Image(int numrows, int numcols, const Color &fillColor = {0, 0, 0});
    /// Constructs an image from a 2D dimensional "array" of colors. Every row
    /// vector must be the same size!
    Image(const std::vector<std::vector<Color>> &);

    /// @returns Tuple {numrows, numcols}
    std::pair<int, int> shape() const;
    /// @returns Number of rows in the image
    int numRows() const;
    /// @returns Number of columns in the image
    int numCols() const;
    /// @param numrows Number of rows in the resized image
    /// @param numcols Number of columns in the resized image
    /// @returns Resized image
    Image resize(int numrows, int numcols) const;

    /// @returns A reference to the color at the specified location
    Color &at(int row, int column);
    /// @returns A const reference to the color at the specified location
    const Color &at(int row, int column) const;

  private:
    /// Image dimensions do not change. Therefore, the number of rows and
    /// columns are const.
    const int m_numrows;
    const int m_numcols;
    std::vector<std::vector<Color>> m_pixels;
};
} // namespace common