// Chris McVickar
#pragma once

<<<<<<< HEAD
#include "Matrix.hpp"
#include "common/common.hpp"
=======
#include "Color.hpp"
#include "Matrix.hpp"
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9

#include <png.h>

#include <functional>
#include <vector>

<<<<<<< HEAD
namespace transmitter {
class Image : public Matrix<common::Color> {
=======
namespace common {
class Image : public Matrix<Color> {
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
  public:
    /// @returns A random image with the specified size
    static Image noise(int numrows, int numcols);

    /// Construct an image from a given size and an optional fill color
<<<<<<< HEAD
    Image(int numrows, int numcols, const common::Color &fillColor = {});
    /// Construct an image from a 2D vector array
    Image(const std::vector<std::vector<common::Color>> &data);
=======
    Image(int numrows, int numcols, const Color &fillColor = {});
    /// Construct an image from a 2D vector array
    Image(const std::vector<std::vector<Color>> &data);
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    Image resize(int numRows, int numCols);

    /// @returns The image with every pixel inverted
    Image invert() const;
    /// @returns The image recolored with the provided color
<<<<<<< HEAD
    Image recolor(const common::Color &) const;
=======
    Image recolor(const Color &) const;
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
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
<<<<<<< HEAD
    Image compress() const;
    /// @returns A 2D vector of encoded colors
    std::vector<uint8_t> encode() const;

  protected:
    Image(const Matrix<common::Color> &);

    /// Applies the function to all pixels
    void apply(const std::function<void(common::Color &)> &);
};
} // namespace transmitter
=======
    /// @returns A 2D vector of encoded colors
    std::vector<uint8_t> encoded() const;

  protected:
    Image(const Matrix<Color> &);

    /// Applies the function to all pixels
    void apply(const std::function<void(Color &)> &);
};
} // namespace common
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
