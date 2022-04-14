#include "Image.hpp"
#include "Matrix.hpp"

#include <png.h>

#include <cinttypes>
#include <iostream>
#include <vector>

namespace common {
Image Image::noise(int numrows, int numcols) {
    Image img(numrows, numcols);
    img.apply([](Color &color) { color = Color::random(); });
    return img;
}

Image::Image(int numrows, int numcols, const Color &fillColor)
    : Matrix<Color>(numrows, numcols) {
    apply([fillColor](Color &color) { color = fillColor; });
}

Image::Image(const std::vector<std::vector<Color>> &data)
    : Matrix<Color>(data) {}

Image Image::resize(int numRows, int numCols) {
    return Image(Matrix<Color>::resize(numRows, numCols));
}

Image::Image(const Matrix<Color> &matrix) : Matrix<Color>(matrix) {}

Image Image::invert() const {
    Image inverted = *this;
    inverted.apply([](Color &color) { color = Color(255, 255, 255) - color; });
    return inverted;
}

Image Image::recolor(const Color &hue) const {
    Image recolored = *this;
    recolored.apply([hue](Color &color) { color = hue * color.intensity(); });
    return recolored;
}

Image Image::grayscale() const {
    Image gray = *this;
    gray.apply([](Color &color) { color = color.grayscale(); });
    return gray;
}

Image Image::saturate(double s) const {
    Image saturated = *this;
    saturated.apply([s](Color &color) { color = color.withSaturation(s); });
    return saturated;
}

Image Image::hue(double h) const {
    Image hued = *this;
    hued.apply([h](Color &color) { color = color.withHue(h); });
    return hued;
}

Image Image::blackAndWhite() const {
    Image bw = *this;
    bw.apply([](Color &color) {
        float brightness = color.intensity();
        if (brightness > .5) {
            color = {255, 255, 255};
        } else {
            color = {0, 0, 0};
        }
    });
    return bw;
}

std::vector<uint8_t> Image::encoded() const {
    std::vector<uint8_t> encoded_img;

    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            encoded_img.push_back(m_data[i][j].encode());
        }
    }
    return encoded_img;
}

void Image::apply(const std::function<void(Color &)> &func) {
    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            func(at(i, j));
        }
    }
}
} // namespace common