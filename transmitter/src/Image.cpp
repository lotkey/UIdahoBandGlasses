/// @author @lotkey Chris McVickar
#include "Image.hpp"
#include "Matrix.hpp"
#include "common.hpp"

#include <cinttypes>
#include <iostream>
#include <vector>

namespace transmitter {
Image Image::noise(int numrows, int numcols)
{
  Image img(numrows, numcols);
  img.apply([](common::Color& color) { color = common::Color::random(); });
  return img;
}

Image::Image(int numrows, int numcols, common::Color const& fillColor) :
    Matrix<common::Color>(numrows, numcols)
{
  apply([fillColor](common::Color& color) { color = fillColor; });
}

Image::Image(std::vector<std::vector<common::Color>> const& data) :
    Matrix<common::Color>(data)
{}

Image Image::resize(int numRows, int numCols)
{
  return Image(Matrix<common::Color>::resize(numRows, numCols));
}

Image::Image(Matrix<common::Color> const& matrix) :
    Matrix<common::Color>(matrix)
{}

Image Image::invert() const
{
  Image inverted = *this;
  inverted.apply(
    [](common::Color& color) { color = common::Color(255, 255, 255) - color; });
  return inverted;
}

Image Image::recolor(common::Color const& hue) const
{
  Image recolored = *this;
  recolored.apply(
    [&hue](common::Color& color) { color = hue * color.intensity(); });
  return recolored;
}

Image Image::grayscale() const
{
  Image gray = *this;
  gray.apply([](common::Color& color) { color = color.grayscale(); });
  return gray;
}

Image Image::saturate(double s) const
{
  Image saturated = *this;
  saturated.apply(
    [s](common::Color& color) { color = color.withSaturation(s); });
  return saturated;
}

Image Image::hue(double h) const
{
  Image hued = *this;
  hued.apply([h](common::Color& color) { color = color.withHue(h); });
  return hued;
}

Image Image::blackAndWhite() const
{
  Image bw = *this;
  bw.apply([](common::Color& color) {
    if (color.intensity() > .5) {
      color = {255, 255, 255};
    } else {
      color = {0, 0, 0};
    }
  });
  return bw;
}

Image Image::compress() const
{
  Image compressed = *this;
  compressed.apply([](common::Color& color) {
    color = common::Color::decode(color.encode());
  });
  return compressed;
}

std::vector<std::uint8_t> Image::encode() const
{
  std::vector<std::uint8_t> encoded_img;

  for (int i = 0; i < numRows(); i++) {
    for (int j = 0; j < numCols(); j++) {
      encoded_img.push_back(m_data[i][j].encode());
    }
  }
  return encoded_img;
}

Image Image::applyToAll(std::function<void(common::Color&)> const& modifier)
{
  Image modified = *this;
  modified.apply(modifier);
  return modified;
}

Image Image::applyToAll(
  std::function<void(common::Color&, int x, int y)> const& modifier)
{
  Image modified = *this;
  modified.apply(modifier);
  return modified;
}
} // namespace transmitter