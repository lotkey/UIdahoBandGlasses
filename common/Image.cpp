#include "Image.hpp"
#include "Matrix.hpp"

namespace common {
Image Image::noise(int numrows, int numcols) {
    Image img(numrows, numcols);
    img.apply([](Color color) { return Color::random(); });
    return img;
}

Image::Image(int numrows, int numcols, const Color &fillColor)
    : Matrix<Color>(numrows, numcols) {
    apply([fillColor](Color color) { return fillColor; });
}

Image::Image(const std::vector<std::vector<Color>> &data)
    : Matrix<Color>(data) {}

Image::Image(const Matrix<Color> &matrix) : Matrix<Color>(matrix) {}

Image Image::invert() const {
    Image inverted = *this;
    inverted.apply([](Color color) { return Color(255, 255, 255) - color; });
    return inverted;
}

Image Image::recolor(const Color &hue) const {
    Image recolored = *this;
    recolored.apply([hue](Color color) { return hue * color.intensity(); });
    return recolored;
}

Image Image::grayscale() const {
    Image gray = *this;
    gray.apply([](Color color) { return color.grayscale(); });
    return gray;
}

Image Image::saturate(double s) const {
    Image saturated = *this;
    saturated.apply([s](Color color) { return color.withSaturation(s); });
    return saturated;
}

Image Image::hue(double h) const {
    Image hued = *this;
    hued.apply([h](Color color) { return color.withHue(h); });
    return hued;
}

void Image::apply(const std::function<Color(Color)> &func) {
    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            at(i, j) = func(at(i, j));
        }
    }
}
} // namespace common