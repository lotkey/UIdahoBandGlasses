#include "Image.hpp"
#include "Matrix.hpp"

namespace common {
Image::Image(int numrows, int numcols, const Color &fillColor)
    : Matrix<Color>(numrows, numcols) {
    for (auto &row : m_data) {
        for (auto &item : row) {
            item = fillColor;
        }
    }
}

Image::Image(const std::vector<std::vector<Color>> &data)
    : Matrix<Color>(data) {}

Image::Image(const Matrix<Color> &matrix) : Matrix<Color>(matrix) {}

Image Image::resize(int numrows, int numcols) const {
    return Matrix<Color>::resize(numrows, numcols);
}

Image Image::invert() const {
    Image inverted = *this;
    for (int i = 0; i < inverted.numRows(); i++) {
        for (int j = 0; j < inverted.numCols(); j++) {
            inverted.at(i, j) = Color(255, 255, 255) - at(i, j);
        }
    }
    return inverted;
}

Image Image::recolor(const Color &color) const {
    Image recolored = *this;
    for (int i = 0; i < recolored.numRows(); i++) {
        for (int j = 0; j < recolored.numCols(); j++) {
            recolored.at(i, j) = color * at(i, j).brightness();
        }
    }
    return recolored;
}
} // namespace common