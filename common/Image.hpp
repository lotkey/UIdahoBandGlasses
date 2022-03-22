#pragma once

#include "Color.hpp"

#include <vector>

namespace common {
class Image {
  public:
    Image(int height, int width);
    Image(const std::vector<std::vector<Color>> &);

    // Lots of stuff to add
    // Ways to access elements WITHOUT resizing image (do const references to
    // vectors allow you to change the elements inside them? if so, return const
    // references to rows instead of just normal references)
    // Resize method
    // To bytes and from bytes?

    const std::vector<Color> at(int row);
    const std::vector<Color> at(int row, int column);

  private:
    const int m_height;
    const int m_width;
    std::vector<std::vector<Color>> m_pixels;
};
} // namespace common