// Chris McVickar
#include "Image.hpp"
#include "Matrix.hpp"
<<<<<<< HEAD
#include "common/common.hpp"
=======
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9

#include <png.h>

#include <cinttypes>
#include <iostream>
#include <vector>

<<<<<<< HEAD
namespace transmitter {
Image Image::noise(int numrows, int numcols) {
    Image img(numrows, numcols);
    img.apply([](common::Color &color) { color = common::Color::random(); });
    return img;
}

Image::Image(int numrows, int numcols, const common::Color &fillColor)
    : Matrix<common::Color>(numrows, numcols) {
    apply([fillColor](common::Color &color) { color = fillColor; });
}

Image::Image(const std::vector<std::vector<common::Color>> &data)
    : Matrix<common::Color>(data) {}

Image Image::resize(int numRows, int numCols) {
    return Image(Matrix<common::Color>::resize(numRows, numCols));
}

Image::Image(const Matrix<common::Color> &matrix)
    : Matrix<common::Color>(matrix) {}

Image Image::invert() const {
    Image inverted = *this;
    inverted.apply([](common::Color &color) {
        color = common::Color(255, 255, 255) - color;
    });
    return inverted;
}

Image Image::recolor(const common::Color &hue) const {
    Image recolored = *this;
    recolored.apply(
        [hue](common::Color &color) { color = hue * color.intensity(); });
=======
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
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    return recolored;
}

Image Image::grayscale() const {
    Image gray = *this;
<<<<<<< HEAD
    gray.apply([](common::Color &color) { color = color.grayscale(); });
=======
    gray.apply([](Color &color) { color = color.grayscale(); });
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    return gray;
}

Image Image::saturate(double s) const {
    Image saturated = *this;
<<<<<<< HEAD
    saturated.apply(
        [s](common::Color &color) { color = color.withSaturation(s); });
=======
    saturated.apply([s](Color &color) { color = color.withSaturation(s); });
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    return saturated;
}

Image Image::hue(double h) const {
    Image hued = *this;
<<<<<<< HEAD
    hued.apply([h](common::Color &color) { color = color.withHue(h); });
=======
    hued.apply([h](Color &color) { color = color.withHue(h); });
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    return hued;
}

Image Image::blackAndWhite() const {
    Image bw = *this;
<<<<<<< HEAD
    bw.apply([](common::Color &color) {
=======
    bw.apply([](Color &color) {
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
        float brightness = color.intensity();
        if (brightness > .5) {
            color = {255, 255, 255};
        } else {
            color = {0, 0, 0};
        }
    });
    return bw;
}

<<<<<<< HEAD
Image Image::compress() const {
    Image compressed = *this;
    compressed.apply([](common::Color &color) {
        color = common::Color::decode(color.encode());
    });
    return compressed;
}

std::vector<uint8_t> Image::encode() const {
=======
std::vector<uint8_t> Image::encoded() const {
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    std::vector<uint8_t> encoded_img;

    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            encoded_img.push_back(m_data[i][j].encode());
        }
    }
    return encoded_img;
}

<<<<<<< HEAD
void Image::apply(const std::function<void(common::Color &)> &func) {
=======
void Image::apply(const std::function<void(Color &)> &func) {
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            func(at(i, j));
        }
    }
}
<<<<<<< HEAD
} // namespace transmitter
=======
} // namespace common
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
