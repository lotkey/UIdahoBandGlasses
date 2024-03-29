/// @author @lotkey Chris McVickar
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

  Image() = default;
  /// Construct an image from a given size and an optional fill color
  Image(int numrows, int numcols, common::Color const& fillColor = {});
  /// Construct an image from a 2D vector array
  Image(std::vector<std::vector<common::Color>> const& data);

  Image resize(int numRows, int numCols);

  /// @returns The image with every pixel inverted
  Image invert() const;
  /// @returns The image recolored with the provided color
  Image recolor(common::Color const&) const;
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
  std::vector<std::uint8_t> encode() const;

  /// Apply a callback to every pixel in the image
  /// @param callback Callback to apply to every pixel. Takes the color of the
  /// pixel as a modifiable reference.
  /// @return An image with the callback applied
  Image applyToAll(std::function<void(common::Color&)> const& callback);
  /// Apply a callback to every pixel in the image
  /// @param callback Callback to apply to every pixel. Takes the color of the
  /// pixel as a modifiable reference and the coordinates of the pixel.
  /// @return An image with the callback applied
  Image applyToAll(
    std::function<void(common::Color&, int x, int y)> const& callback);

  /// @param array std::uint8_t array that encode() updates
  /// @param n size of array
  /// @returns true if encode() was successful, false else
  bool encode(std::uint8_t array[], int n) const;

protected:
  Image(Matrix<common::Color> const&);
};
} // namespace transmitter