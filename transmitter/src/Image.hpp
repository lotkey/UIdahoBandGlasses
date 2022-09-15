// Chris McVickar
#pragma once

#include "Matrix.hpp"
#include "common.hpp"

#include <functional>
#include <vector>

namespace transmitter {
class Image : public Matrix<common::Color> {
public:
  /// @returns A random image with the specified size
  static Image noise(int numrows, int numcols);

  /// Construct an image from a given size and an optional fill color
  Image(int numrows, int numcols, common::Color const& fillColor = {});
  /// Construct an image from a 2D vector array
  Image(std::vector<std::vector<common::Color>> const& data);

  Image resize(int numRows, int numCols);

  /// @returns The image with every pixel inverted
  Image invert() const;
  /// @returns The image recolored with the provided color
  Image recolor(common::Color const&) const;
  /// @returns The image with the color changed.
  Image changeColor(common::Color const&) const;
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
  Image compress() const;
  /// @returns A 2D vector of encoded colors
  [[nodiscard]] std::vector<uint8_t> encode() const;

  Image applyToAll(std::function<void(common::Color&)> const&);
  Image applyToAll(std::function<void(common::Color&, int x, int y)> const&);

  /// @param array uint8_t array that encode() updates
  /// @param n size of array
  /// @returns true if encode() was successful, false else
  bool encode(uint8_t array[], int n) const;

protected:
  Image(Matrix<common::Color> const&);
};
} // namespace transmitter