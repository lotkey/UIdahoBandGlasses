#include "config.hpp"

#include <stdexcept>

namespace transmitter::config {
static int s_imageWidth = -1;
static int s_imageHeight = -1;

void setImageWidth(int width)
{
  if (-1 == s_imageWidth) {
    s_imageWidth = width;
  } else {
    throw std::runtime_error("Image width is already set.");
  }
}

void setImageHeight(int height)
{
  if (-1 == s_imageHeight) {
    s_imageHeight = height;
  } else {
    throw std::runtime_error("Image height is already set.");
  }
}

int imageWidth()
{
  if (-1 == s_imageWidth) {
    throw std::runtime_error("Image width is not set.");
  }
  return s_imageWidth;
}

int imageHeight()
{
  if (-1 == s_imageHeight) {
    throw std::runtime_error("Image height is not set.");
  }
  return s_imageHeight;
}
} // namespace transmitter::config