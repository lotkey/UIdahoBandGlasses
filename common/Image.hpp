#pragma once

#include "Color.hpp"
#include "Matrix.hpp"

namespace common {
class Image : public Matrix<Color> {
  public:
    Image(int numrows, int numcols, const Color &fillColor = {0, 0, 0});
    Image(const std::vector<std::vector<Color>> &data);

    Image resize(int numrows, int numcols) const;
    Image invert() const;
    Image recolor(const Color &) const;

  protected:
    Image(const Matrix<Color> &);
};
} // namespace common